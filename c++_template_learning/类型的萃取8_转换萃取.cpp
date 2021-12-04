#include <iostream>

// 除了可以被用来访问主参数类型的某些特性，萃取还可以被用来做类型转换，
// 比如为某个类 型添加或移除引用、const 以及 volatile 限制符;

// 删除引用
// 我们可以实现一个 RemoveReferenceT 萃取，用它将引用类型转换成其底层对象或者函数的类型，对于非引用类型则保持不变;

template<typename T>
struct RemoveReferenceT 
{ 
	using Type = T; 
};

template<typename T>
struct RemoveReferenceT<T&>
{ 
	using Type = T; 
};

template<typename T>
struct RemoveReferenceT<T&&> {
	using Type = T; 
};

template<typename T> 
using RemoveReference = typename RemoveReference<T>::Type;

// C++标准库提供了一个相应的 std::remove_reference<> 萃取;


// 添加引用
// 我们也可以给一个已有类型添加左值或者右值引用;
template<typename T> 
struct AddLValueReferenceT 
{ 
	using Type = T&; 
};

template<typename T>
using AddLValueReference = typename AddLValueReferenceT<T>::Type;

template<typename T> 
struct AddRValueReferenceT
{
	using Type = T&&;
};

template<typename T>
using AddRValueReferenceT = T&;

template<typename T>
using AddRValueReferenceTT = T&&;

// C++ 标准库中也提供了与之相应的类型萃取: std::add_lvalue_reference<> 和 std::add_rvalue_reference<>;

// 移除限制符
// 转换萃取可以分解或者引入任意种类的复合类型，并不仅限于引用;
// 比如，如果一个类型中 存在 const 限制符，我们可以将其移除：
template<typename T>
struct RemoveConstT 
{ 
	using Type = T;
};

template<typename T>
struct RemoveConstT<T const> 
{ 
	using Type = T;
};

template<typename T> 
using RemoveConst = typename RemoveConstT<T>::Type;

// 转换萃取可以是多功能的，比如创建一个可以被用来移除 const 和 volatile 的 RemoveCVT 萃取:
template<typename T> 
struct RemoveCVT : RemoveConstT<typename RemoveVolatileT<T>::Type> 
{

};

template<typename T> 
using RemoveCV = typename RemoveCVT<T>::Type;

// 还可以仿照标准库中的 std::decay 实现一个退化萃取：
// 首先我们对非数组、非函数的情 况进行定义，该情况只需要删除 const 和 volatile 限制符即可：
template<typename T> 
struct DecayT : RemoveCVT<T> 
{

};

// 然后我们处理数组到指针的退化，这需要用偏特化来处理所有的数组类型（有界和无界数组）：
template<typename T> 
struct DecayT<T[]> 
{ 
	using Type = T*; 
};

template<typename T, std::size_t N>
struct DecayT<T[N]>
{
	using Type = T*; 
};

// 最后来处理函数到指针的退化，这需要应对所有的函数类型，不管是什么返回类型以及有多数参数：
template<typename R, typename... Args> 
struct DecayT<R(Args...)>
{
	using Type = R(*)(Args...);
};

template<typename R, typename... Args>
struct DecayT<R(Args..., ...)> {
	using Type = R(*)(Args..., ...);
};

template <typename T> 
using Decay = typename DecayT<T>::Type;

int main()
{

	return 0;
}