#include <cctype>
#include <cstdio>
#include <cstdlib>

enum class Token {
    TOK_LPAR,
    TOK_RPAR,
    TOK_NUM,
    TOK_DIV,
    TOK_MUL,
    TOK_ADD,
    TOK_SUB,
    TOK_EOF,
    TOK_LIN,
    TOK_QIT,
    TOK_ERR,
    TOK_NOP
};

/* PDA which records previous events and results */
struct State {
    Token ops; // The operator or token that initiated the current state
    double val; // The result or value associated with the current state
};

#define MAX_CALLS 3

struct {
    /* The stack which holds the state and its previous caller */
    State calls[MAX_CALLS];
    /* Recursive calls are simulated with a Push Down Automata */
    int nb_calls; // Number of active calls in the stack
} stack;

#define STACK_PREV() stack.calls[stack.nb_calls - 1]
#define STACK_TOP() stack.calls[stack.nb_calls]

double num;
FILE *fp = stdin;
Token tok;

Token read_tok() {
    /* Purge all white space and read the next character */
    int c = fgetc(fp);
    while (true) {
        switch (c) {
        case '\r':
        case ' ':
        case '\t':
            c = fgetc(fp);
            continue; // Skip whitespace characters
        case EOF:
            return Token::TOK_EOF;
        }
        break; // Exit loop when a non-whitespace character is found
    }

    if (isdigit(c)) {
        ungetc(c, fp);
        char buf[256] = {0};
        int i = 0;
        fscanf(fp, "%255[0-9.]", buf);
        num = atof(buf);
        return Token::TOK_NUM;
    }

    switch (c) {
    case '\n':
        return Token::TOK_LIN;
    case '*':
        return Token::TOK_MUL;
    case '/':
        return Token::TOK_DIV;
    case '+':
        return Token::TOK_ADD;
    case '-':
        return Token::TOK_SUB;
    case 'q':
        return Token::TOK_QIT;
    case '(':
        return Token::TOK_LPAR;
    case ')':
        return Token::TOK_RPAR;
    case EOF:
        return Token::TOK_EOF;
    default:
        fprintf(stderr, "illegal character, '%c'\n", c);
        return Token::TOK_ERR;
    }
}

// Clean the stack by reading tokens until a new line is encountered
void clean() {
    while (tok != Token::TOK_LIN) {
        tok = read_tok();
    }
}

double apply_ops(double a, double b, Token op) {
    // Apply the operation based on the token
    switch (op) {
    case Token::TOK_MUL:
        return a * b;
    case Token::TOK_DIV:
        return a / b;
    case Token::TOK_ADD:
        return a + b;
    case Token::TOK_SUB:
        return a - b;
    /*case Token::TOK_RPAR:  // No operator, just return the number*/
    /*    return b;*/
    default:
        fprintf(stderr, "error: unknown operator\n");
        exit(-1);
    }
}

int precedence(Token op) {
    // Determine the precedence of operators
    if (op == Token::TOK_ADD || op == Token::TOK_SUB) {
        return 1; // Lower precedence
    }
    if (op == Token::TOK_MUL || op == Token::TOK_DIV) {
        return 2; // Higher precedence
    }
    return 0; // No precedence
}

int should_quit() {
    return tok == Token::TOK_QIT || tok == Token::TOK_EOF
        || tok == Token::TOK_ERR;
}

// calculator PDA logic
void calculate() {
    printf("Enter q to quit\n");

    while (true) {
        printf("> ");
        fflush(stdout);

        tok = read_tok();
        if (should_quit()) {
            return;
        }

        if (tok == Token::TOK_LIN) {
            continue;
        }

        // Process tokens until a new line or quit command is encountered
        while (tok != Token::TOK_LIN && !should_quit()) {
            // Check for stack overflow
            if (stack.nb_calls >= MAX_CALLS) {
                goto overflow;
            }

            switch (tok) {
            case Token::TOK_NUM:
                // Store the number in the current stack top
                STACK_TOP().val = num;
                break;
            case Token::TOK_LPAR:
                // Set the current operation to right parenthesis and increase
                // call count
                STACK_TOP().ops = Token::TOK_RPAR;
                stack.nb_calls++;
                break;
            case Token::TOK_RPAR:
                // Process operations until a left parenthesis is found
                if (stack.nb_calls == 0) {
                    clean();
                    fprintf(stderr, "error: mismatched parenthesis\n");
                    goto next;
                }
                while (stack.nb_calls >= 1 && STACK_TOP().ops != Token::TOK_RPAR
                       && STACK_PREV().ops != Token::TOK_RPAR) {
                    STACK_PREV().val = apply_ops(
                        STACK_PREV().val,
                        STACK_TOP().val,
                        STACK_PREV().ops
                    );
                    stack.nb_calls--;
                }
                STACK_PREV().val = STACK_TOP().val;
                STACK_PREV().ops = Token::TOK_NOP;
                stack.nb_calls--;
                break;
            case Token::TOK_ADD:
            case Token::TOK_SUB:
            case Token::TOK_DIV:
            case Token::TOK_MUL:
                // Evaluate operations based on precedence
                if (stack.nb_calls >= 1
                    && stack.calls[stack.nb_calls].ops != Token::TOK_RPAR
                    && precedence(tok)
                        <= precedence(stack.calls[stack.nb_calls].ops)) {
                    STACK_PREV().val = apply_ops(
                        STACK_PREV().val,
                        STACK_TOP().val,
                        STACK_PREV().ops
                    );
                    stack.nb_calls--;
                }
                // Set the current operation and increase call count
                STACK_TOP().ops = tok;
                stack.nb_calls++;
            default:
                break;
            }
            // Read the next token
            tok = read_tok();
        }
        if (should_quit()) {
            return;
        }

        // Evaluate remaining operations in the stack
        while (stack.nb_calls >= 1) {
            if (STACK_PREV().ops != Token::TOK_RPAR) {
                STACK_PREV().val = apply_ops(
                    STACK_PREV().val,
                    STACK_TOP().val,
                    STACK_PREV().ops
                );
                stack.nb_calls--;
            } else {
                fprintf(stderr, "error: mismatched parenthesis\n");
                goto next;
            }
        }
        // At this point, the final result is stored in stack.calls[0].val
        printf("= %f\n", stack.calls[0].val);
        goto next;

overflow:
        fprintf(stderr, "error: too many expression. resetting...\n");
        clean();
next:
        for (int i = 0; i < MAX_CALLS; i++) {
            stack.calls[i].val = 0;
            stack.calls[i].ops = Token::TOK_NOP;
        }
        stack.nb_calls = 0;
    }
}

int main() {
    // init stack
    for (int i = 0; i < MAX_CALLS; i++) {
        stack.calls[i].val = 0;
        stack.calls[i].ops = Token::TOK_NOP;
    }
    stack.nb_calls = 0;

    calculate();
    return 0;
}
