#include "stack.h"

Stack::Stack(int size) {
    this->size = size;
    this->index = 0;
    this->ptr = new int[size];
}

Stack::Stack(const Stack &s) {
    this->size = s.size;
    this->index = 0;
    this->ptr = new int[size];
    for (int i = 0; i < size; i++) {
        ptr[i] = s.ptr[i];
    }
}

Stack::Stack(Stack &&s) {
    // release the pointer of other
    int *new_ptr = s.ptr;
    s.ptr = nullptr;

    // assign the pointer to this
    this->ptr = new_ptr;

    // other recsources cleanup
    this->size = s.size;
    this->index = 0;
    s.size = 0;
    s.index = 0;
}

Stack::~Stack() {
    delete[] ptr;
}

Stack &Stack::operator=(const Stack &s) {
    if (this == &s) {
        return *this;
    }

    this->ptr = s.ptr;
    this->size = s.size;
    this->index = 0;

    return *this;
}

Stack &Stack::operator=(Stack &&s) {
    if(this == &s){
        return *this;
    }
    this->ptr = s.ptr;
    this->size = s.size;
    this->index = 0;
    s.ptr = nullptr;
    return *this;
}

void Stack::push(int val) {
    if (index < size) {
        ptr[index++] = val;
    }
}

int Stack::pop() {
    if (!index) {
        return -1;
    }

    return ptr[--index];
}

int Stack::top() {
    if (!index) {
        return -1;
    }

    return ptr[index - 1];
}

bool Stack::empty() {
    return !index;
}
