#include <iostream>
#include <string>

// 按引用传递不会拷贝对象（因为形参将引用被传递的实参）。
// 而且按引用传递时参数类型也不会退化（decay）;
// 但并不是在所有情况下都能使用按引用传递，即使在能使用的地方，有时候被推断出来的模板参数类型也会带来不少问题。

// 情形一：按 const 引用传递：
// 为了避免（不必要的）拷贝，在传递非临时对象作为参数时，可以使用 const 引用传递；
template<typename T> 
void printR1(T const& arg) // 这个模板永远不会拷贝被传递对象（不管拷贝成本是高还是低）；
{
	std::cout << "typeof arg = " << typeid(arg).name() << std::endl;
}

// 要点一：按引用传递参数时，其类型不会退化（decay）；
// 也就是说不会把裸数组转换为指针，也不会移除 const 和 volatile 等限制符。
// 而且由于调用参数被声明为 T const &，被推断出来的模 板参数 T 的类型将不包含 const；
// 见情形二：

// 情形三：按非 const 引用传递：
// 如果想通过调用参数来返回变量值（比如修改被传递变量的值），
// 就需要使用非 const 引用 （要么就使用指针）。
// 同样这时候也不会拷贝被传递的参数。被调用的函数模板可以直接访问被传递的参数;
template<typename T> 
void outR(T& arg) 
{ 
	std::cout << "typeof arg = " << typeid(arg).name() << std::endl;

	// 由于这里传递的非 const 类型的裸数组，其类型也不会 decay，
	// 这样就可以修改数组中元素的值，也可以处理数组的长度。
	if (std::is_array<T>::value) { 
		std::cout << "got array of " << std::extent<T>::value << " elems" << std::endl;
	}
}

// 情形五：
// 如果想禁止想非 const 应用传递 const 对象，有如下选择:
// 1. 使用 static_assert 触发一个编译期错误:
template<typename T> 
void outR1(T& arg)
{
	static_assert(!std::is_const<T>::value, "out parameter of foo<T>(T&) is const"); 
	std::cout << "typeof arg = " << typeid(arg).name() << std::endl;
 }
// 2. 通过使用 std::enable_if<>禁用该情况下的模板：
template<typename T, typename = std::enable_if_t<!std::is_const<T>::value>>
void outR2(T& arg) 
{
	std::cout << "typeof arg = " << typeid(arg).name() << std::endl;
}

int main()
{
	// 情形一：
	std::string returnString();
	std::string s = "hi"; 
	printR1(s); // no copy 
	printR1(std::string("hi")); // no copy 
	//printR1(returnString); // no copy // 这句话运行不了，很奇怪；
	printR1(std::move(s)); // no copy

	std::cout << "------------------------------------" << std::endl;
	// 情形二：
	std::string const c = "hi"; 
	printR1(c); // T deduced as std::string, arg is std::string const&；
	printR1("hi"); // T deduced as char[3], arg is char const(&)[3]；
	int arr[4]; 
	printR1(arr); // T deduced as int[4], arg is int const(&)[4]；

	std::cout << "------------------------------------" << std::endl;
	// 情形三
	outR(s); //OK: T deduced as std::string, arg is std::string&；
	// 注意对于 outR()，通常不允许将临时变量（prvalue）或者通过 std::move()处理过的已存在的变量（xvalue）用作其参数；
	//outR(std::string("hi")); //ERROR: not allowed to pass a temporary (prvalue)；
	//outR(returnString()); // ERROR: not allowed to pass a temporary (prvalue)；
	//outR(std::move(s)); // ERROR: not allowed to pass an xvalue；

	// 同样可以传递非 const 类型的裸数组，其类型也不会 decay： 
	outR(arr); // OK: T deduced as int[4], arg is int(&)[4];

	std::cout << "------------------------------------" << std::endl;
	// 情形四：
	// 此时如果传递的参数是 const 的，arg 的类型就有可能被推断为 const 引用，
	// 也就是说这时可以传递一个右值（rvalue）作为参数，但是模板所期望的参数类型却是左值（lvalue）；
	
	const int d = 100;
	// 这里理论上会将 d 推断为 const int, 但是运行结果还是 int，很奇怪。
	outR(d); // OK: T deduced as int const; 
	outR(std::move(d)); // OK: T deduced as int; 
	//outR(100); //这里报错：error C2664: “void outR<int>(T &)”: 无法将参数 1 从“int”转换为“int &”；
	
	// 情形五:
	//outR1(d); //加了编译器assert，报错 error C2338: out parameter of foo<T>(T&) is const；
	//outR2(d); // 加了 SFINAE， 编译期就不通过

	return 0;
}