#include <iostream>

// 使用引用调用（Call-By-Reference）的一个原因是可以对参数进行完美转发（Perfect Forward）;
// 但是请记住在使用转发引用时（Forwarding Reference，被定义成一个模板参数的右值引用（rvalue Reference）），
// 有它自己特殊的规则;

// 情形一：
// 下面这样写可以将任意类型的参数传递给转发引用，而且和往常的按引用传递一样，
// 都不会创建被传递参数的备份：
template<typename T>
void passR(T&& arg) // 注意按照《Effective Mordern C++》上写，这个叫做--万能引用；
{ 
	std::cout << "typeof arg = " << typeid(arg).name() << std::endl;
}

int main()
{
	std::string s = "hi"; 
	std::string returnString();
	passR(s); // OK: T deduced as std::string& (also the type of arg) 
	passR(std::string("hi")); // OK: T deduced as std::string, arg is std::string&& 
	//passR(returnString()); // OK: T deduced as std::string, arg is std::string&& 
	passR(std::move(s)); // OK: T deduced as std::string, arg is std::string&& 
	int arr[4];
	passR(arr); // OK: T deduced as int(&)[4] (also the type of arg）

	// 但是，这种情况下类型推断的特殊规则可能会导致意想不到的结果：
	std::string const c = "hi"; 
	passR(c); //OK: T deduced as std::string const& 
	passR("hi"); //OK: T deduced as char const(&)[3] (also the type of arg) 
	int arr[4]; passR(arr); //OK: T deduced as int (&)[4] (also the type of arg)
	// 在以上三种情况中，都可以在 passR()内部从 arg 的类型得知被传递的参数是一个右值 （rvalue）
	// 还是一个 const 或者非 const 的左值（lvalue）。这是唯一一种可以传递一个参数， 
	// 并用它来区分以上三种情况的方法


	return 0;
}