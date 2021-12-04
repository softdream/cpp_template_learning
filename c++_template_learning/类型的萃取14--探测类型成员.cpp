#include <iostream>
#include <type_traits>

// 另一种对基于 SFINAE 的萃取的应用是，创建一个可以判断一个给定类型 T 是否
// 含有名为 X 的成员（类型或者非类型成员）的萃取。

// 探测类型成员（Detecting Member Types）:

// 首先定义一个可以判断给定类型 T 是否含有类型成员 size_type 的萃取:

// defines true_type and false_type 
// helper to ignore any number of template parameters:
template<typename ...> 
using VoidT = void;

// primary template: 
template<typename, typename = VoidT<>>
struct HasSizeTypeT : std::false_type
{

};

// partial specialization (may be SFINAE’d away): 
template<typename T> 
struct HasSizeTypeT<T, VoidT<typename T::size_type>> : std::true_type
	// 该条件只有在 T 含有类型成员 size_type 的时候才有效;
	// 如果对于某个类型 T，该条件无效，那么 SFINAE 会使偏特化实现被丢弃，我们就退回到主模板的情况;
{

} ;


struct CX { 
	using size_type = std::size_t; 
};

// 在处理引用类型的时候可能会遇到让人意外的事情。比如，虽然如下的代码可以正常工作： 
struct CXR {
	using size_type = char&; // Note: type size_type is a reference type
};

// 引用类型确实没有成员，但是当我们使用引用的时候，结果表达式的类型是引用所指向的类型，
// 因此我们可能会希望，当我们传递进来的模板参数是引用类型 的时候，依然根据其指向的类型做判断。
// 为了这一目的，可以在 HasSizeTypeT 的偏特化中使用我们之前介绍的 RemoveReference 萃取：
template<typename T> 
struct HasSizeTypeT<T, VoidT<std::remove_reference_t<T>::size_type>> : std::true_type
{ 

};

int main()
{
	std::cout << HasSizeTypeT<int>::value << std::endl; // false

	std::cout << HasSizeTypeT<CX>::value << std::endl; // true

	std::cout << HasSizeTypeT<CXR>::value << std::endl; // OK: prints true

	// 下面与之类似的代码却不会输出我们所期望的结果
	std::cout << HasSizeTypeT<CXR&>::value << std::endl; // OOPS: prints false 
	std::cout << HasSizeTypeT<CXR&&>::value << std::endl; // OOPS: prints false

	return 0;
}