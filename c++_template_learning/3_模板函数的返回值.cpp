#include <iostream>
#include <type_traits>

/* 例程 3 函数模板的返回值 */

// 情形一：函数的返回值类型和形参的类型一样时 //
template<typename T>
T max1(const T& a, const T& b) // 
{
	return a < b ? b : a;
}

// 情形二：当模板参数和调用参数之间没有必然的联系，且模板参数不能确定的时候，就要显式的指明模板参数//
template<typename T, typename RT> // 这里新加了一个模板参数作为返回值类型；
RT max2(const T& a, const T& b)	  // 缺点就是在调用时无法做自动类型推导了。
								  // 模板类型推断不会考虑返回类型，而 RT 又没有被用作调用参数的类型。因此 RT 不会被推断。这样就必须显式的指明模板参数的类型；
{
	return a < b ? b : a;
}

// 情形三：为了支持返回值类型推断，c++11的写法如下：
template<typename T1, typename T2>
auto max3(const T1& a, const T2& b)->decltype(a < b ? b : a)
// 在 C++11 中，尾置返回类型（trailing return type）允许我们使用函数的调用参数。 也就是说，我们可以基于运算符?:的结果声明返回类型
{
	return a < b ? b : a;
}

// 情形四：为了支持返回值类型推断，c++14的写法如下：
template<typename T1, typename T2>
auto max4(const T1& a, const T2& b)
// 在 C++14 中，只要使用 auto 关键字即可，编译器会自动做返回值类型推导。
{
	return a < b ? b : a;
}

// 但是在情形三和四中某些情况下会有一个严重的问题：由于 T1、T2 可能是引用类型，返回类型就也可能被推断为引用类型。因此你应该返回的是 decay 后的 T；
// 情形五：对返回值类型做退化
template<typename T1, typename T2>
auto max5(const T1& a, const T2& b)->typename std::decay<decltype(a < b ? b : a)>::type
// 这里用到了标准库中的 std::decay 对返回值类型进行“退化”操作，去除引用、const、volitile等类型特性
{
	return a < b ? b : a;
}

// 情形六：将返回类型声明为公共类型（Common Type）
// 从 C++11 开始，标准库提供了一种指定“更一般类型”的方式。std::common_type<>::type 产生的类型是他的两个模板参数的公共类型。
template<typename T1, typename T2>
std::common_type_t<T1, T2> max6(const T1& a, const T2& b)
// std::common_type 也是一个类型萃取（type trait），定义在<type_traits>中，它返回一个结构体，结构体的 type 成员被用作目标类型；
{
	return a < b ? b : a;
}


int main()
{
	
	std::cout << "1: max1(2, 5) = " << max1(2, 5) << std::endl;

	// 这里必须显式指明返回值类型
	std::cout << "2: max2(2, 5) = " << max2<int, int>(2, 5) << std::endl;
	
	std::cout << "3: max3(3,100) = " << max3(3, 100) << std::endl;


	std::cout << "4: max4(30,100) = " << max4(30, 100) << std::endl;

	std::cout << "5: max5(30,100) = " << max5(30, 100) << std::endl;

	std::cout << "6: max6(30,100) = " << max6(30, 100) << std::endl;

	return 0;
}
