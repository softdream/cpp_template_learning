#include <iostream>
#include <array>
#include <type_traits>

// 另一个可以被用来处理多个类型的类型函数的例子是返回值类型萃取。
// 在编写操作符模板的时候它们会很有用。

// 例如：我们来写一个可以对两个 Array 容器求和的函数模板：
template<typename T, int N = 10> 
std::array<T, N> operator+ (std::array<T, N> const& a, std::array<T, N> const& b)
{

}

// 但是由于语言本身允许我们对一个 char 型数值和一个整形数值求和，
// 我们自然也很希望能够对 Array 也执行这种混合类型（mixed-type）的操作。
// 这样我们就要处理该如何决定相关模板的返回值的问题;
// 一个可以解决上述问题的方式就是返回值类型模板：
// template<typename T1, typename T2> 
// std::array<typename PlusResultT<T1, T2>::Type> operator+ (std::array<T1> const& a, std::array<T2> const& b)
// {

// }

// 其中的 PlusResultT 萃取会自行判断通过+操作符对两种类型（可能是不同类型）的数值求和所得到的类型:
template<typename T1, typename T2> 
struct PlusResultT
{
	using Type = decltype(T1() + T2());
};
// 这一萃取模板通过使用 decltype 来计算表达式 T1()+T2()的类型，将决定结果类型这一艰巨的工作
// （包括处理类型增进规则（promotion rules）和运算符重载）留给了编译器;

template<typename T1, typename T2> 
using PlusResult = typename PlusResultT<T1, T2>::Type;

/*template<typename T1, typename T2, int N = 10> 
std::array<typename PlusResultT<T1, T2>::Type, N> operator+ (std::array<T1, N> const& a, std::array<T2, N> const& b)
{
	return a + b;
}*/

// 这里，decltype 可能会保留了过多的信息，比如，我们的 PlusResultT 可能会返回一个引用类型，
// 但是我们的 Array 模板却很可能不是为引用类型设计的。
// 这是可以用之前说的去除引用，去除常量性等萃取技术了：
/*template<typename T1, typename T2, int N = 10>
std::array<std::remove_const_t<std::remove_reference_t<PlusResult<T1, T2>>>, N> operator+ (std::array<T1, N> const& a, std::array<T2, N> const& b)
{
	std::array<std::remove_const_t<std::remove_reference_t<PlusResult<T1, T2>>>, N> ret;

	for (int i = 0; i < N; i++) {
		ret[i] = a[i] + b[i];
	}

	return ret;
}*/
// 上述这样写依然很麻烦，改写为：
template<typename T1, typename T2, int N = 10, 
		 typename RT = std::array<std::remove_const_t<std::remove_reference_t<PlusResult<T1, T2>>>, N>
		>
 RT operator+ (std::array<T1, N> const& a, std::array<T2, N> const& b)
{
	RT ret;

	for (int i = 0; i < N; i++) {
		ret[i] = a[i] + b[i];
	}

	return ret;
}

// 由于表达式 T1() + T2()试图对类型 T1 和 T2 的数值进行值初始化，这两个类型必须要有可访问的、未被删除的默认构造函数（或者是非 class 类型）。
// Array 类本身可能并没有要求其元素类型可以被进行值初始化，因此这是一个额外的、不必要的限制。

// 在我们可以很简单的在不需要构造函数的情况下计算+表达式的值，方法就是使用一个可以为一个给定类型 T 生成数值的函数。
// 为了这一目的，C++标准提供了 std::declval<>;

// 表达式 declval<>可以在不需要使用默认构造函数（或者其它任意操作）的情况下为类型 T 生成一个值。

// 有了 declval，我们就可以不用在 PlusResultT 中使用值初始化了:
template<typename T1, typename T2> 
struct PlusResultT2 
{ 
	using Type = decltype(std::declval<T1>() + std::declval<T2>()); 
}; 
template<typename T1, typename T2>
using PlusResult2 = typename PlusResultT2<T1, T2>::Type;

int main()
{
	std::array<int, 10> a = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	std::array<char, 10> b = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	auto c = a + b;

	for (auto it : c) {
		std::cout << it << std::endl;
	}

	return 0;
}