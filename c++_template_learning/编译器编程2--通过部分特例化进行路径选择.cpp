#include <iostream>

// 诸如 isPrime()这种在编译期进行相关测试的功能，
// 有一个有意思的应用场景：可以在编译期间通过部分特例化在不同的实现方案之间做选择。

constexpr bool doIsPrime(unsigned p, unsigned d) // p: number to check, d: current divisor
{
	return d != 2
		? (p % d != 0) && doIsPrime(p, d - 1) // check this and smaller divisors
		: (p % 2 != 0); // end recursion if divisor is 2 
}
constexpr bool isPrime(unsigned p)
{
	return p < 4 ? !(p < 2) // handle special cases
		: doIsPrime(p, p / 2); // start recursion with divisor from p/2
}

// 比如，可以以一个非类型模板参数是不是质数为条件，在不同的模板之间做选择:
// 先定义一个基础模板；
template<int SZ, bool = isPrime(SZ)> 
struct Helper;

// SZ 不是质数的偏特化
template<int SZ>
struct Helper<SZ, false>
{
	bool value = false;
};

// SZ 是质数的偏特化
template<int SZ>
struct Helper<SZ, true>
{
	bool value = true;
};

// 这一偏特例化 的使用方法，被广泛用于基于模板参数属性，在不同模板实现方案之间做选择。

template<int SZ>
void test()
{
	Helper<SZ> h;
	if (h.value == true) {
		std::cout << " SZ 是质数的偏特化" << std::endl;
	}
	else {
		std::cout << " SZ 不是质数的偏特化" << std::endl;
	}

}

int main()
{

	test<53>();

	return 0;
}