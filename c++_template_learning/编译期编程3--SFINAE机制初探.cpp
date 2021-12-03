#include <iostream>
#include <vector>

// 在 C++中，重载函数以支持不同类型的参数是很常规的操作。当编译器遇到一个重载函数的调用时，
// 它必须分别考虑每一个重载版本，以选择其中类型最匹配的那一个;

// 在一个函数调用的备选方案中包含函数模板时，
// 1. 编译器首先要决定应该将什么样的模板参数用于各种模板方案，
// 2. 然后用这些参数替换函数模板的参数列表以及返回类型，
// 3. 最后评估替换后的函数模板和这个调用的匹配情况（就像常规函数一样）;

// 但是这一替换过程可能会遇到 问题：替换产生的结果可能没有意义。
// 不过这一类型的替换不会导致错误，C++语言规则要求忽略掉这一类型的替换结果。

// 这一原理被称为 SFINAE，代表的是“substitution failure is not an error”。

// 例：
// number of elements in a raw array: 
template<typename T, unsigned N> 
std::size_t len ( T(&)[N] ) 
{
	return N; 
}

// number of elements for a type having size_type: 
template<typename T>
typename T::size_type len (T const& t) 
{ 
	return t.size(); 
}

// 这里定义了两个接受一个泛型参数的函数模板 len()：
// 1. 第一个函数模板的参数类型是 T (&)[N]，也就是说它是一个包含了 N 个 T 型元素的数组;
// 2. 第二个函数模板的参数类型就是简单的 T，除了返回类型要是 T::size_type 之外没有别的 限制，这要求被传递的参数类型必须有一个 size_type 成员;

int main()
{
	// 当传递的参数是裸数组或者字符串常量时，只有那个为裸数组定义的函数模板能够匹配;
	int a[10]; 
	std::cout << len(a); // OK: only len() for array matches 
	std::cout << len("tmp"); //OK: only len() for array matches

	// 如果只是从函数签名来看的话，对第二个函数模板也可以分别用 int[10]和 char const [4]替换类型参数 T，
	// 但是这种替换在处理返回类型 T::size_type 时会导致错误。因此对于这两个调用， 第二个函数模板会被忽略掉。

	// 如果传递 std::vector<>作为参数的话，则只有第二个模板参数能够匹配： 
	std::vector<int> v; 
	std::cout << len(v); // OK: only len() for a type with size_type matches

	// 如果传递的是裸指针话，以上两个模板都不会被匹配上（但是不会因此而报错）。
	// 此时编译期会抱怨说没有发现合适的 len()函数： 
	int* p; 
	//std::cout << len(p); // ERROR: no matching len() function found

	// 但是这和传递一个有 size_type 成员但是没有 size()成员函数的情况不一样。
	// 比如如果传递的参数是 std::allocator<>: 
	std::allocator<int> x; 
	std::cout << len(x); // ERROR: len() function found, but can’t size()
	// 此时编译器会匹配到第二个函数模板。因此不会报错说没有发现合适的 len()函数，
	// 而是会报一个编译期错误说对 std::allocator<int>而言 size()是一个无效调用。
	// 此时第二个模板函数不会被忽略掉。



	return 0;
}