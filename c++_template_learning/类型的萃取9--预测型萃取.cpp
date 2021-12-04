#include <iostream>

// 可以设计基于多个参数的类型函数。这同样会引出另外一种特殊的类型萃取--类型预测（产生一个 bool 数值的类型函数）;

// IsSameT : IsSameT 将判断两个类型是否相同;
// 这里的主模板说明通常我们传递进来的两个类型是不同的，因此其 value 成员是 false
template<typename T1, typename T2> 
struct IsSameT 
{ 
	static constexpr bool value = false;
};

// 通过使用偏特化，当遇到传递进来的两个相同类型的特殊情况，value 成员就是 true 的;
template<typename T> 
struct IsSameT<T, T> 
{ 
	static constexpr bool value = true; 
};

// 对于产生一个常量的萃取，我们没法为之定义一个别名模板，
// 但是可以为之定义一个扮演可相同角色的 constexpr 的变量模板:
//template<typename T1, typename T2> 
//constexpr bool isSame = IsSameT<T1, T2>::value;// 这句话在 vs2015 下报错：“consexpr”在此处无效；

// C++标准库提供了与之相应的 std::is_same<>;

// true_type 和 false_type:
// 通过为可能的输出结果 true 和 false 提供不同的类型，我们可以大大的提高对 IsSameT 的定义:
// 如果我们声明一个 BoolConstant 模板以及两个可能的实例 TrueType 和 FalseType：
template<bool val> 
struct BoolConstant 
{ 
	using Type = BoolConstant<val>;
	static constexpr bool value = val;
};

using TrueType = BoolConstant<true>;
using FalseType = BoolConstant<false>;

// 就可以基于两个类型是否匹配，让相应的 IsSameT 分别继承自 TrueType 和 FalseType:
template<typename T1, typename T2> 
struct IsSameT2 : FalseType
{

}; 

template<typename T> 
struct IsSameT2<T, T> : TrueType
{

};

// 现在 IsSameT<T, int> 的返回类型会被隐式的转换成其基类 TrueType 或者 FalseType，
// 这样就不仅提供了相应的 value 成员，还允许在编译期间将相应的需求派发到对应的函数实现
// 或者类模板的偏特化上。
// 例如：
template<typename T> 
void fooImpl(T, TrueType) {
	std::cout << "fooImpl(T,true) for int called\n";
}

template<typename T> 
void fooImpl(T, FalseType) 
{ 
	std::cout << "fooImpl(T,false) for other type called\n";
}

template<typename T> 
void foo(T t) 
{
	fooImpl(t, IsSameT2<T, int>{}); // choose impl. depending on whether T is int
}

// 这一技术被称为标记派发（tag dispatching）;

// 从 C++11 开始 C++ 标准库在<type_traits>中提供了相应的类型：std::true_type 和 std::false_type;

int main()
{

	foo(42); // calls fooImpl(42, TrueType)
	foo(7.7); // calls fooImpl(42, FalseType)

	return 0;
}