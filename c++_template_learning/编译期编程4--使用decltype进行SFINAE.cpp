#include <iostream>
#include <vector>

// 对于有些限制条件，并不总是很容易地就能找到并设计出合适的表达式来 SFINAE 掉函数模板。
// 比如，对于有 size_type 成员但是没有 size()成员函数的参数类型，我们想要保证会忽略掉函数模板 len()。
// 如果没有在函数声明中以某种方式要求 size()成员函数必须存在，这个函数模板就会被选择并在实例化过程中导致错误：
// template<typename T> 
// typename T::size_type len(T const& t)
// {
// 		return t.size();
// }
// std::allocator<int> x; 
// std::cout << len(x) << ’\n’; //ERROR: len() selected, but x has no size();

// 处理这一情况有一种常用模式或者说习惯用法:
// 1. 通过尾置返回类型语法（trailing return type syntax）来指定返回类型（在函数名前使用 auto，并在函数名后面的->后指定返回类型）:
// 2. 通过 decltype 和逗号运算符定义返回类型;
// 3. 将所有需要成立的表达式放在逗号运算符的前面（为了预防可能会发生的运算符被重载的情况，需要将这些表达式的类型转换为 void）;
// 4. 在逗号运算符的末尾定义一个类型为返回类型的对象;

template<typename T> 
auto len(T const& t) -> decltype( (void)(t.size()), T::size_type() ) 
{ 
	return t.size(); 
}
// 类型指示符 decltype 的操作数是一组用逗号隔开的表达式，
// 因此最后一个表达式 T::size_type() 会产生一个类型为返回类型的对象（decltype 会将其转换为返回类型）;
// 而在最后一个逗号前面的所有表达式都必须成立，在这个例子中逗号前面只有 t.size()。
// 之所以将其类型转换为 void，是为了避免因为用户重载了该表达式对应类型的逗号运算符而导致的不确定性。

int main()
{

	std::allocator<int> x;
	// std::cout << len(x) << ’\n’; // 这样 len(x) 就不会实例化上述的模板函数了，匹配会忽略, 编译器会报错。
	
	return 0;
}


