#include <iostream>

// 返回值也可以被按引用或者按值返回。但是按引用返回可能会带来一些麻烦，
// 因为它所引用的对象不能被很好的控制;
// 不过在日常编程中，也有一些情况更倾向于按引用返回:
// 1. 返回容器或者字符串中的元素（比如通过[]运算符或者 front()方法访问元素）;
// 2. 允许修改类对象的成员;
// 3. 为链式调用返回一个对象（比如>>和<<运算符以及赋值运算符）;
// 4. 另外对成员的只读访问，通常也通过返回 const 引用实现。

// 要点一：在需要确保函数返回值是按值返回的情况下：
// 使用函数模板 T 作为返回类型并不能保证返回值不会是引用，
// 因为 T 在某些情况下会被隐式推断为引用类型：
// 例一：
template<typename T> 
T retR(T&& p) // p is a forwarding reference 
{ 
	return p; // OOPS: returns by reference when called for lvalues 
}

// 即使函数模板被声明为按值传递，也可以显式地将 T 指定为引用类型:
// 例二：
template<typename T> 
T retV(T p) //Note: T might become a reference
{ 
	return p; // OOPS: returns a reference if T is a reference 
}

// 安全起见，有两种选择:
// 1. 用类型萃取 std::remove_reference<>将 T 转为非引用类型:
template<typename T> 
typename std::remove_reference<T>::type 
retV1(T p) 
{
	return p; // always returns by value 
}

// 2. 将返回类型声明为 auto，从而让编译器去推断返回类型，这是因为 auto 也会导致类型退化;
template<typename T>
auto retV2(T p) // by-value return type deduced by compiler 
{ 
	return p; // always returns by value
}

int main()
{
	// 例二：
	int x = 10; 
	std::cout<<"type = "<<typeid(retV<int&>(x)).name()<<std::endl; // retT() instantiated for T as int&



	return 0;
}