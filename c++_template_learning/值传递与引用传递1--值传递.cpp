#include <iostream>

// 当按值传递参数时，原则上所有的参数都会被拷贝。
// 因此每一个参数都会是被传递实参的一 份拷贝。
// 对于 class 的对象，参数会通过 class 的拷贝构造函数来做初始化。

// 调用拷贝构造函数的成本可能很高。但是有多种方法可以避免按值传递的高昂成本：
// 事实上 编译器可以通过移动语义（move semantics）来优化掉对象的拷贝，
// 这样即使是对复杂类型 的拷贝，其成本也不会很高。

// 例一：
template<typename T> 
void printV(T arg) 
{
	std::cout << "arg = " << arg.c_str() << std::endl;
}

// 要点一：当按值传递参数时，参数类型会退化（decay）。 
// 也就是说，裸数组会退化成指针，const 和 volatile 等限制符会被删除;
// 这一行为继承自 C 语言，既有优点也有缺点。
// 通常它会简化对被传递字符串常量的处理，
// 但是缺点是在 printV()内部无法区分被传递的是一个对象的指针还是一个存储一组对象的数组;
template<typename T>
void printV2(T arg)
{
	std::cout << "type of arg = " << typeid(arg).name() << std::endl;
}


int main()
{
	std::string s = "hi";
	
	// 这里被传递的参数是左值（lvalue），因此拷贝构造函数会被调用；
	printV( s ); // 在传递字符串时，arg 变成 s 的一份拷贝。
				 // 此时这一拷贝是通过 std::string 的拷贝构造函数创 建的，
				 // 这可能会是一个成本很高的操作，因为这个拷贝操作会对源对象做一次深拷贝，
				 // 它需要开辟足够的内存来存储字符串的值。

	// 但是并不是所有的情况都会调用拷贝构造函数:
	// 以下两个函数被传递的参数是纯右值（prvalue，pure right value，临时对象或者某个函数的返回值），
	// 此时编译器会优化参数传递，使得拷贝构造函数不会被调用。
	printV(std::string("hi")); //copying usually optimized away (if not, move constructor);
	printV(std::move(s)); // move constructor
	// 从 C++17 开始，C++标准要求这一优化方案必须被实现。
	// 在 C++17 之前，如果编译器没有优化掉这一类拷贝，它至少应该先尝试使用移动语义，
	// 这通常也会使拷贝成本变得比较低廉。

	std::cout << "---------------------------------" << std::endl;

	std::string const c = "hello";
	printV2(c); // c decays so that arg has type std::string;
	printV2("hello"); //decays to pointer so that arg has type char const*;
	int arr[4]; 
	printV2(arr); // decays to pointer so that arg has type int *;

	return 0;
}