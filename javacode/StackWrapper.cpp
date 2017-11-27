#include "StackWrapper.h"

template <typename T>
StackWrapper<T>::StackWrapper() {
} 

template <typename T>
void StackWrapper<T>::push_wrapper(T&& obj) {

	//m_stack.push(std::forward<T>(obj)); //determine if to copy or to movej
	m_stack.push(obj); //determine if to copy or to movej
}
template <typename T>
T StackWrapper<T>::pop_wrapper() {
	T whoami = m_stack.top(); //mark object as moveable
	m_stack.pop();
	return whoami;

}

