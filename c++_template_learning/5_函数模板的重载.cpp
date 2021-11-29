#include <iostream>
#include <type_traits>

/* 例程 5 函数模板的重载 */

int max(const int& a, const int& b)
{
	std::cout << "no template " << std::endl;
	return a < b ? b : a;
}

// 情形一：一个非模板函数可以和一个与其同名的函数模板共存，并且这个同名的函数模板可以被实例化为与非模板函数具有相同类型的调用参数；
// 注意：在所有其它因素都相同的情况下，模板解析过程将优先选择非模板函数，而不是从模板实例化出来的函数；
template<typename T>
T max(const T& a, const T& b) 
{
	std::cout << "template " << std::endl;
	return a < b ? b : a;
}

int main()
{
	// 这个调用的是非模板函数
	std::cout << "max(5, 100) = " << ::max(5, 100) << std::endl << std::endl; 

	// 在模板参数推断时不允许自动类型转换(即隐式转换)，而常规函数是允许的，所以这里调用的也是非模板函数
	std::cout << "max('a', 100) = " << ::max('a', 100) << std::endl;

	return 0;
}