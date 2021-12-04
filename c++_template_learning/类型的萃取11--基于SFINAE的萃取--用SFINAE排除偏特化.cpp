#include <iostream>
#include <type_traits>

// 另一种实现基于 SFINAE 的萃取的方式会用到偏特化;
// 依然以判断类型 T 是否是可以被默认初始化为例子：

// 别名模板，helper to ignore any number of template parameters: 
template<typename ...> 
using VoidT = void;

// primary template:
template<typename, typename = VoidT<>> 
struct IsDefaultConstructibleT : std::false_type 
{

};

// partial specialization (may be SFINAE’d away): 
template<typename T> 
struct IsDefaultConstructibleT<T, VoidT<decltype( T() )>> : std::true_type 
{

} ;
// 这里只需要一个类型构造： decltype(T()) 这样就可以检测类型 T 是否是可以被默认初始化的；

// 如果对于某个特定的类型 T，其默认构造函数是无效的，此时 SIFINEAE 就是使该偏特化被丢弃掉，
// 并最终使用主模板。否则该偏特化就是有效的，并且会被选用;


struct S
{
	S() = delete; // S 的默认构造函数被禁了；
};

int main()
{
	IsDefaultConstructibleT<int>::value; // Ok, true;

	IsDefaultConstructibleT<S>::value;

	return 0;
}