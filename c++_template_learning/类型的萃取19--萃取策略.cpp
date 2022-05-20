#include <iostream>
#include <type_traits>

// 到目前为止，我们例子中的萃取模板被用来判断模板参数的特性：
// 它们代表的是哪一种类型， 作用于该类型数值的操作符的返回值的类型，以及其它特性。
// 这一类萃取被称为特性萃取 （property traits）。

// 做为对比，某些萃取定义的是该如何处理某些类型。我们称之为策略萃取（policy traits）。

// 虽然特性萃取通常都可以被实现为类型函数，策略萃取却通常将策略包装进成员函数中。

// 情形一：只读参数类型
// 在 C++和 C 中，函数的调用参数（call parameters）默认情况下是按照值传递的。
// 这意味着， 调用函数计算出来的参数的值，会被拷贝到由被调用函数控制的位置。
// 大部分程序员都知道， 对于比较大的结构体，这一拷贝的成本会非常高，
// 因此对于这一类结构体最好能够将其按照 常量引用（reference-to-const）或者
// 是 C 中的常量指针（pointer-to-const）进行传递;

// 当然，有了模板之后事情要变得更加微妙一些：
// 我们事先并不知道用来替换模板参数的类型将会是多大。而且，事情也并不是仅仅依赖于结构体的大小：
// 即使是比较小的结构体，其拷贝构造函数的成本也可能会很高，这种情况下我们应对选择按常量引用传递。

// 这一类问题通常应当用策略萃取模板（一个类型函数）来处理：
// 该函数将预期的参数类型 T 映射到最佳的参数类型 T 或者是 T const&。
// 作为第一步的近似，主模板会将大小不大于两个指针的类型按值进行传递，
// 对于其它所有类型都按照常量引用进行传递：

template<typename T> 
struct RParam 
{
	using Type = typename IfThenElseT<sizeof(T) <= 2 * sizeof(void*), T, T const&>::Type; 
};

// 另一方面，对于那些另 sizeof 运算符返回一个很小的值，
// 但是拷贝构造函数成本却很高的容器类型，我们可能需要分别对它们进行特化或者偏特化:
template<typename T>
struct RParam<Array<T>> 
{
	using Type = Array<T> const&; 
};

// 由于这一类类型在 C++中很常见，如果只将那些拥有简单拷贝以及移动构造函数的类型按值进行传递，
// 当需要考虑性能因素时，再选择性的将其它一些 class 类型加入按值传递的行列 
// （C++标准库中包含了 std::is_trivially_copy_constructible 和 std::is_trivially_move_constructible 类型萃取）。
template<typename T> 
struct RParam 
{
	using Type = IfThenElse<(sizeof(T) <= 2 * sizeof(void*) && std::is_trivially_copy_constructible<T>::value && std::is_trivially_move_constructible<T>::value), T, T const&>; 
};

// 无论采用哪一种方式，现在该策略都可以被集成到萃取模板的定义中，客户也可以用它们去实现更好的效果。
// 比如，假设我们有两个 class，对于其中一个 class 我们指明要按值传递只读参数：
class MyClass1 {
public:
	MyClass1() 
	{
	
	}
	
	MyClass1(MyClass1 const&)
	{
		std::cout << "MyClass1 copy constructor called\n";
	}

};

class MyClass2 
{ 
public: 
	MyClass2() 
	{ 
	
	}
	
	MyClass2(MyClass2 const&) 
	{
		std::cout << "MyClass2 copy constructor called\n";
	}

};

// pass MyClass2 objects with RParam<> by value 4
template<> 
class RParam<MyClass2> 
{
public: 
	using Type = MyClass2; 
};


