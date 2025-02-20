#include "core.hpp"
#include "ggml.h"
#include <cassert>
#include <cstdint>
#include <string>
#include <memory>

namespace sep {

RunState::RunState(Config *config) : config(config) {

	uint16_t kv_dim		= (config->dim * config->n_kv_heads) / config->n_heads;
	uint16_t dim		= config->dim;
	uint16_t hidden_dim = config->hidden_dim;
	uint16_t n_layers	= config->n_layers;
	uint32_t cache_size	= sizeof(float) *n_layers * config->seq_len * kv_dim;

	// alloc Optensors' buffer
	x = new float [dim * sizeof(float)];
	xb			= new float[dim* sizeof(float)];
	xb2			= new float[dim* sizeof(float)];
	hb			= new float[hidden_dim* sizeof(float)];
	hb2			= new float[hidden_dim* sizeof(float)];
	q			= new float[dim* sizeof(float)];
	key_cache	= new float[cache_size];
	value_cache = new float[cache_size];
	att			= new float[sizeof(float) *config->n_heads * config->seq_len];
	logits		= new float[sizeof(float) *config->vocab_size];
}

RunState::~RunState() {
	delete [] x;
	delete [] xb;
	delete [] xb2;
	delete [] hb;
	delete [] hb2;
	delete [] q;
	delete [] key_cache;
	delete [] value_cache;
	delete [] att;
	delete [] logits;
}

Transformer::Transformer(std::string filename) : filename(filename),ggml_ctx_(nullptr) {
	{
		gguf_init_params params = {.no_alloc = false, .ctx = &ggml_ctx_};
		gguf_ctx_				= gguf_init_from_file(filename.c_str(), params);
		assert(gguf_ctx_ != nullptr);
		assert(ggml_ctx_ != nullptr);
	}
	config = new Config(gguf_ctx_);
	weight = new Weight(ggml_ctx_, config->n_layers);
	state  = new RunState(config);
}

Transformer::~Transformer() {
	gguf_free(gguf_ctx_);
	delete config;
	delete weight;
	delete state;
}

void Transformer::multihead_attention(uint32_t pos, uint64_t loff, Config &p, RunState &s) {

	auto dim	   = p.dim;
	auto kv_dim	   = (p.dim * p.n_kv_heads) / p.n_heads;
	auto kv_mul	   = p.n_heads / p.n_kv_heads;
	auto head_size = dim / p.n_heads;

	uint32_t h = 0;
	for (h = 0; h < p.n_heads; h++) {
		auto q	 = s.q + h * head_size;
		auto att = s.att + h * p.seq_len;

		for (auto t = 0; t <= pos; t++) {
			auto k	   = s.key_cache + loff + t * kv_dim + (h / kv_mul) * head_size;
			auto score = 0.0f;

			for (auto i = 0; i < head_size; i++) {
				score += q[i] * k[i];
			}

			score /= sqrtf(head_size);
			att[t] = score;
		}

		softmax(att, pos + 1);

		auto xb = s.xb + h * head_size;
		memset(xb, 0, head_size * sizeof(float));

		for (auto t = 0; t <= pos; t++) {
			auto v = s.value_cache + loff + t * kv_dim + (h / kv_mul) * head_size;
			auto a = att[t];

			for (auto i = 0; i < head_size; i++) {
				xb[i] += a * v[i];
			}
		}
	}
}

void Transformer::attention(int pos, int L) {
	auto p = config;
	auto s = state;

	auto w = weight;

	auto kv_dim = (p->dim * p->n_kv_heads) / p->n_heads;

	rmsnorm(s->xb, s->x, w->lw[L].attn_norm, p->dim);

	uint64_t loff = L * p->seq_len * kv_dim;
	s->k		  = s->key_cache + loff + pos * kv_dim;
	s->v		  = s->value_cache + loff + pos * kv_dim;
	// QKV
	matmul(s->q, s->xb, w->lw[L].attn_q, p->dim, p->dim);
	matmul(s->k, s->xb, w->lw[L].attn_k, p->dim, kv_dim);
	matmul(s->v, s->xb, w->lw[L].attn_v, p->dim, kv_dim);
	// position embedding
	rope(pos, p, s);

	multihead_attention(pos, loff, *p, *s);

	matmul(s->xb2, s->xb, w->lw[L].attn_output, p->dim, p->dim);

	// residual connection
	for (auto i = 0; i < p->dim; i++) {
		s->x[i] += s->xb2[i];
	}
	
}

void Transformer::ffn(int L) {
	auto p = config;
	auto w = weight;
	auto s = state;

	rmsnorm(s->xb, s->x, w->lw[L].ffn_norm, p->dim);

	// ffn_gate and ffn_up
	matmul(s->hb, s->xb, w->lw[L].ffn_gate, p->dim, p->hidden_dim);
	matmul(s->hb2, s->xb, w->lw[L].ffn_up, p->dim, p->hidden_dim);

	for (auto i = 0; i < p->hidden_dim; i++) {
		float val = s->hb[i];
		// silu(x)=x*σ(x), where σ(x) is the logistic sigmoid
		val *= (1.0f / (1.0f + expf(-val)));
		// elementwise multiply with w3(x)
		val *= s->hb2[i];
		s->hb[i] = val;
	}
	// ffn_down
	matmul(s->xb, s->hb, w->lw[L].ffn_down, p->hidden_dim, p->dim);

	// residual connection
	for (int i = 0; i < p->dim; i++) {
		s->x[i] += s->xb[i];
	}
}

float *Transformer::forward(int token, int pos) {
	auto p = config;
	auto w = weight;
	auto s = state;
	float *logits = new float[p->vocab_size];

	auto dim = p->dim;

	// 1. input embedding
	float *content_row = w->token_embedding_table + token * dim;
	memcpy(s->x,content_row, dim * sizeof(float));

	for (auto L = 0; L < p->n_layers; L++) {
		// 2. attention
		attention(pos, L);
		// 3. ffn
		ffn(L);
	}

	rmsnorm(s->x, s->x, w->rms_final_weight, dim);

	matmul(logits, s->x, w->output_weight, dim, p->vocab_size);

	return logits;
}

void Transformer::generate(Tokenizer *tk, Sampler *sampler, std::string prompt, int steps) {
	// encode the (string) prompt into tokens sequence
	int num_prompt_tokens = 0;
	auto prompt_tokens	  = tk->tokenize(prompt, true);
	num_prompt_tokens	  = prompt_tokens.size();

	if (num_prompt_tokens < 1) {
		fmt::println(stderr, "something is wrong, expected at least 1 prompt token\n");
		exit(EXIT_FAILURE);
	}
	// start the main loop
	int next;					   // will store the next token in the sequence
	auto token = prompt_tokens[0]; // kick off with the first token in the prompt
	int pos	   = 0;				   // position in the sequence
	while (pos < steps) {

		// forward the transformer to get logits for the next token
		float *logits = forward(token, pos);

		// advance the state machine
		if (pos < num_prompt_tokens - 1) {
			// if we are still processing the input prompt, force the next
			// prompt token
			next = prompt_tokens[pos + 1];
		} else {
			// otherwise sample the next token from the logits
			next = sampler->sample(logits);
		}
		delete [] logits;
		pos++;

		// data-dependent terminating condition: the BOS token delimits
		// sequences
		if (next == tk->bos_token()) {
			break;
		}

		// print the token as string, decode it with the Tokenizer object
		auto piece = tk->to_string(next);
		fmt::print("{}", piece);
		fflush(stdout);
		token = next;
	}
	fmt::println("");
}

} // namespace sep
