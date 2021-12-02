#include <iostream>
#include <type_traits>

// 从 C++11 开始，通过 C++标准库提供的辅助模板 std::enable_if<>，可以在某些编译期条件下忽略掉函数模板。

// 例一：
template<typename T>
//typename std::enable_if<(sizeof(T) > 4)>::type // c++ 11
std::enable_if_t<(sizeof(T) > 4)> // c++ 14 
fun( T a )// 这一模板定义会在 sizeof(T) > 4 不成立的时候被忽略掉
{
	std::cout << "test" << std::endl;
}

// 也就是说 std::enable_if<>是一种类型萃取（type trait），它会根据一个作为其（第一个）模板参数的编译期表达式决定其行为:
// 1. 如果这个表达式结果为 true，它的 type 成员会返回一个类型:
//		a) 如果没有第二个模板参数，返回类型是 void;
//		b) 如果有第二个模板参数，返回类型是其第二个参数的类型。
// 2. 如果表达式结果 false，则其成员类型是未定义的。根据模板的一个叫做 SFINAE （substitute failure is not an error）的规则， 
// 这会导致包含 std::enable_if<>表达式的函数模板被忽略掉。

// 例二：如果给 std::enable_if<>或者 std::enable_if_t<>传递第二个模板参数：
// 那么在 sizeof(T) > 4 时，enable_if 会被扩展成其第二个模板参数
template<typename T>
std::enable_if_t<(sizeof(T) > 4), T>
fun2( T a )
{
	return a;
}

// 但是由于将 enable_if 表达式放在声明的中间不是一个明智的做法，
// 因此使用 std::enable_if<> 的更常见的方法是使用一个额外的、有默认值的模板参数：
// 例三：使用模板参数简化
template<typename T, typename RT = std::enable_if_t<(sizeof(T) > 4), T>>
RT fun3(T a)
{
	return a;
}
// 或者你可以用别名模板（alias template）给它定义一个别名：
template<typename T>
using ReturnType = std::enable_if_t < (sizeof(T) > 4), T > ;

template<typename T>
ReturnType<T> fun4(T a) // fun4 和 fun3 一样，只是写法不同而已；
{
	return a;
}


int main()
{
	// 例一：
	double a = 10;
	fun(a); // sizoef(double) = 8,可以使用这个模板函数；

	int b = 10;
	//fun(b);// error C2893: 未能使函数模板“enable_if<_Test,_Ty>::type fun(T)”专用化;

	// 例二：
	std::cout << fun2(10.0) << std::endl;

	int d = 10;
	//std::cout << fun2(d) << std::endl; // error C2893: 未能使函数模板“enable_if<_Test,T>::type fun2(T)”专用化;

	// 例三：
	double e = 10.0;
	std::cout << fun3(e) << std::endl;// 这个用法和例二一样；
	

	// 
	double f = 10.0;
	std::cout << fun4(f) << std::endl;

	return 0;
}