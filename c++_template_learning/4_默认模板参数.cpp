#include <iostream>

/* 默认模板参数：可以给模板参数指定默认值。这些默认值被称为默认模板参数并且可以用于任意类型的模板 */

// 情形一：使用 decltype
// 注意：这一实现方式要求我们能够调用两个模板参数的默认构造参数，即这的 T1(), T2()
template<typename T1, typename T2, typename RT = std::decay_t<decltype( true ? T1() : T2() )>>
RT max7(const T1& a, const T2& b)
	// 这里可以显式的指定 RT 的类型，也可以使用为它设立的一个默认类型，这个默认类型是从 T1，T2 退化出来的
{
	return a < b ? b : a;
}

// 情形二：我们也可以利用类型萃取 std::common_type<>作为返回类型的默认值
template<typename T1, typename T2, typename RT = std::common_type_t<T1, T2>>
RT max8(const T1& a, const T2& b)
// 注意 在这里 std::common_type<>也是会做类型退化的。
{
	return a < b ? b : a;
}

int main()
{

	std::cout << "7: max7 " << max7(3, 50) << std::endl;

	auto rt2 = max8(3, 50);
	std::cout << "8: max8 " << rt2 << std::endl;

	auto rt3 = max8<int, int, double>(3, 100);// 这样写的问题在于，为了显式指出返回类型，我们必须显式的指出全部三个模板参数的类型；
	std::cout << "8: max8 = " << rt3 << std::endl;

	return 0;
}