#include <iostream>
#include <typeinfo>
using namespace std;

int main() {

	auto i{27};
	auto x = { 27 };
	auto integer_x = 27;


	cout << typeid(i).name() << endl;
	cout << typeid(x).name() << endl;
	cout << typeid(integer_x).name() << endl;

}
