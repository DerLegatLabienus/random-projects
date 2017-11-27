#include <memory>
#include <iostream>
template<typename T>
struct shared_ptr : std::shared_ptr<T>
{
	template<typename...Args>
	/*explicit*/ shared_ptr(Args &&... args)
		: std::shared_ptr<T>(std::forward<Args>(args)...)
	{
		std::cout << "ctor" << std::endl;
	}

	shared_ptr & operator=(const shared_ptr & rhs) {
		std::cout << "const lvalue" << std::endl;
		return *this;
	}

	shared_ptr & operator=(shared_ptr && rhs) {
		std::cout << "rvalue" << std::endl;
		return *this;
	}
};

struct A {};

struct ConvertsToPtr
{
	shared_ptr<A> ptr = shared_ptr<A>(new A());
	operator shared_ptr<A> const &() const {
		std::cout << "ConvertsToPtr operator" << std::endl;
		return ptr; }
};

int main()
{
	shared_ptr<A> ptr;
	ptr  = ConvertsToPtr(); // error here
	std::cout << std::endl;	
	return 0;
}
