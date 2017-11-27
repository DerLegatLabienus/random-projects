//#ifdef STACK_JAVA
//#include <gcj/cni.h>
//#include <java/lang/System.h>
//#include <java/util/Stack.h>
//#include <java/io/PrintStream.h>
//#include <java/lang/Throwable.h>
//#else 
#ifndef HEADER_
#define HEADER_
#include <stack>
#include <iostream>
//#endif

using namespace std;
template <typename T>
class StackWrapper {
//	public :java::util::Stack s;

	private:
		std::stack<T> m_stack{};

	public :
		StackWrapper();
		void push_wrapper(T&& obj);
		T pop_wrapper();
};

#include "StackWrapper.cpp"
#endif
