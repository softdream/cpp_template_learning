#include <iostream>

void test( int &a )
{
	std::cout << "左值引用" << std::endl;
}

void test( int &&a )
{
	std::cout << "右值引用" << std::endl;
}

template<typename T>
void fun1( T&& a )  // 万能引用
{
	test(a);
}

template<typename T>
void fun2(T&& a)  // 万能引用
{
	test(a);
}

template<typename T>
void fun3(T&& a)  // 万能引用
{
	test(std::forward<T>(a));
}

/*----------------- 注意 ----------------*/
/*当我们使用了万能引用时，即使可以同时匹配左值、右值，但需要转发参数给其他函数时，会丢失引用性质。
这主要是因为形参是个左值，从而无法判断到底匹配的是个左值还是右值。

因为右值是无法取地址的，而一旦这个右值被右值引用，该引用可以被取地址，所以此时这个右值引用被当做是左值。也就是说，右值引用本身就是个左值，只是他可以绑定右值。
————————————————*/

void fun4( int && a) // 右值引用
{
	test(a);
}

void func5(int && a) // 右值引用
{
	test(std::forward<int>(a));
}

void func6(int && a)
{
	test( std::move(a) );
}

int main()
{
	int a = 10;
	fun1(a); // 传进去的是左值， 这里调用的是左值引用版本

	fun2(std::move(a)); // 传进去的是右值，调用的还是左值引用版本

	fun3(std::move(a)); // 这里用了完美转发，传进去的是右值，调用的也是右值
	fun3(a); // 这里用了完美转发，传进去的是左值，调用的也是左值版本

	fun4(std::move(a)); // 这里传进去的是右值，但是没用完美转发，还是调用左值版本

	func5( std::move(a) );// 这里传进去的是右值，使用了完美转发，则调用右值版本

	func6(std::move(a));

	return 0;
}