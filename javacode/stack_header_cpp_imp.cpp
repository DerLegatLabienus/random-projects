#include "StackWrapper.h"
#include "stack_header_cpp.h"


void *createVector_C() {return new StackWrapper<int>();}

void destroyVector_C(void *obj) {
	delete (static_cast<StackWrapper<int> *>(obj));
}
void push_C(void *obj, int elem) {
	StackWrapper<int> *stack = (static_cast<StackWrapper<int> *>(obj));
	stack->push_wrapper(std::move(elem));
}

int pop_C(void *obj) {
	return ((static_cast<StackWrapper<int> *>(obj))->pop_wrapper());
}
