#include <iostream>

// 从 C++14 开始，变量也可以被某种类型参数化。称为变量模板。

// 例一：
// 可以通过下面的代码定义 pi，但是参数化了其类型：
template<typename T> 
T pi{ 3.1415926535897932385 };

// 例二
template<typename T> 
T val{}; // zero initialized value 

int main()
{
	val<long> = 100;

	return 0;
}