#include <iostream>

/* 例程 2 模板类型推断 */


/* 求两个数中的最大值*/
template<typename T>
T max1(T& a, T& b) // 如果使用引用传参，任何类型转换都是不被允许的
{
	std::cout << "type of a = " << typeid(a).name() << std::endl
	std::cout << "type of b = " << typeid(b).name() << std::endl;

	return a < b ? b : a;
}

template<typename T>
T max2(T a, T b) // 如果按值传递，则允许退化（decay）这种简单的类型转换。
{				 // decay是指：const 和 volatile 限制符会被忽略，引用被转换成被引用的类型，
				 // raw array 和函数被转换为相应的指针类型。
	std::cout << "type of a = " << typeid(a).name() << std::endl;
	std::cout << "type of b = " << typeid(b).name() << std::endl;

	return a < b ? b : a;
}

int main()
{
	// 例程 2 调用
	
	const int& a = 40;
	const int& b = 20;

	std::cout<<typeid(a).name()<<std::endl;

	max1(a, b);
	max2(a, b);

	return 0;
}