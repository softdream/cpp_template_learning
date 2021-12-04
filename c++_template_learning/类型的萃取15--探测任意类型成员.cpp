#include <iostream>
#include <type_traits> // for true_type, false_type, and void_t

// 在定义了诸如 HasSizeTypeT 的萃取之后，我们会很自然的想到该如何将该萃取参数化，以对 任意名称的类型成员做探测。
// 目前这一功能只能通过宏来实现，因为还没有语言机制可以被用来描述“潜在” 的名字。当前不使用宏的、与该功能最接近的方法是使用泛型 lambda；

// 如下的宏可以满足我们的需求：
#define DEFINE_HAS_MEMBER(Member) \
template<typename, typename = std::void_t<>> \
struct HasMemberT_##Member \
: std::false_type { }; \
template<typename T> \
struct HasMemberT_##Member<T, std::void_t<decltype(&T::Member)>> \
: std::true_type { } // ; intentionally skipped

int main()
{

	return 0;
}