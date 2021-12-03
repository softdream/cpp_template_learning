#include <iostream>

// 传统上我们在 C 和 C++里定义的函数可以被更明确的称为:
// 值函数（value functions）：它们接收一些值作为参数并返回一个值作为结果。 
// 对于模板，我们还可以定义类型函数（type functions）：它们接收一些类型作为参数并返回一个类型或者常量作为结果。

// 一个很有用的内置类型函数是 sizeof，它返回了一个代表了给定类型大小（单位是 byte）的常数。
// 类模板依然可以被用作类型函数。此时类型函数的参数是模板参数，其结果被提取为成员类型或者成员常量。
// 比如，sizeof 运算符可以被作为如下接口提供;

template<typename T> 
struct TypeSize 
{
	static std::size_t const value = sizeof(T);
};

// TypeSize 是一个模板， 也可以被作为 模板模板参数传递。

int main() 
{ 
	std::cout << "TypeSize<int>::value = " << TypeSize<int>::value << std::endl;

	return 0;
}

