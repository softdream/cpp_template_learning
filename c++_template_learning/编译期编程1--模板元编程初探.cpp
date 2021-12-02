#include <iostream>

// 模板的实例化发生在编译期间（而动态语言的泛型是在程序运行期间决定的）。
// 事实证明 C++模板的某些特性可以和实例化过程相结合，这样就产生了一种 C++自己内部的原始递归的“编程语言”。
// 因此模板可以用来“计算一个程序的结果”;

// 例一：c++11以前语法实现编译器计算--判断质数
template<unsigned p, unsigned d> // p: number to check, d: current divisor
struct DoIsPrime {
	static constexpr bool value = (p % d != 0) && DoIsPrime<p, d - 1>::value; // 递归
};

template<unsigned p> // end recursion if divisor is 2 
struct DoIsPrime<p,2> { 
	static constexpr bool value = (p % 2 != 0); 
};

template<unsigned p> // primary template 
struct IsPrime { // start recursion with divisor from p/2: 
	static constexpr bool value = DoIsPrime<p, p / 2>::value; 
};

// special cases (to avoid endless recursion with template instantiation): 
template<> struct IsPrime<0> { 
	static constexpr bool value = false; 
}; 
template<> struct IsPrime<1> { 
	static constexpr bool value = false; 
}; 
template<> struct IsPrime<2> { 
	static constexpr bool value = true;
}; 
template<> struct IsPrime<3> { 
	static constexpr bool value = true;
};

// C++11 引入了一个叫做 constexpr 的新特性，它大大简化了各种类型的编译期计算。
// 如果给定了合适的输入，constexpr 函数就可以在编译期间完成相应的计算；
// 例二：在 C++11 中，判断一个数是不是质数的实现方式如下

constexpr bool doIsPrime1(unsigned p, unsigned d) // p: number to check, d: current divisor
{
	return d != 2 
		? (p % d != 0) && doIsPrime1(p, d - 1) // check this and smaller divisors
		: (p % 2!=0); // end recursion if divisor is 2 
}
constexpr bool isPrime1(unsigned p)
{
	return p < 4 ? !(p < 2) // handle special cases
		: doIsPrime1(p,p / 2); // start recursion with divisor from p/2
}

// 在 C++14 中，constexpr 函数可以使用常规 C++代码中大部分的控制结构;
// 注意 vs2015 没有实现完 c++14 的特性
/*constexpr bool isPrime(unsigned int p) 
{
	for (unsigned int d = 2; d <= p / 2; ++d) {
		if (p % d == 0) {
			return false; // found divisor without remainder}
		}
	}
	return p > 1; // no divisor without remainder found
}*/

int main()
{
	IsPrime<56>::value; // 这个值编译期间就能计算出来了；

	isPrime1(56);

	return 0;
}