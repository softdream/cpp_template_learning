#include <iostream>
#include <type_traits>

// 标准库提供了各种各样的被称为类型萃取（type traits）的工具，它们可以被用来计算以及修改类型。
// 这样就可以在实例化的时候让泛型代码适应各种类型或者对不同的类型做出不同的响应。

template<typename T> 
class C
{ 
	// ensure that T is not void (ignoring const or volatile): 
	static_assert(!std::is_same<std::remove_cv_t<T>, void>::value, "invalid instantiation of class C for void type");

public:
	template<typename V> 
	void f( V&& v ) // 万能引用
	{
		if constexpr(std::is_reference<T>::value) { // special code if T is a reference type

		}

		if constexpr(std::is_convertible<std::decay_t<V>, T>::value) { // special code if V is convertible to T 

		}

		if constexpr(std::has_virtual_destructor<V>::value) {  // special code if V has virtual destructor

		}
	}

};

// 函数模板 std::declval()可以被用作某一类型的对象的引用的占位符。
// 该函数模板没有定义， 因此不能被调用（也不会创建对象）。
// 因此它只能被用作不会被计算的操作数（比如 decltype 和 sizeof）。
// 也因此，在不创建对象的情况下，依然可以假设有相应类型的可用对象。
template<typename T1, typename T2, 
		 typename RT = std::decay_t<decltype(true ? std::declval<T1>() : std::declval<T2>())>
		>
RT max (T1 a, T2 b) 
{ 
	return b < a ? a : b; 
}


int main()
{

	return 0;
}