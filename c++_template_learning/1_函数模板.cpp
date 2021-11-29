#ifdef DEBUG 

#include <iostream>

/* 例程 1 最简单的函数模板：求两个数中的最大值 */
template<typename T>
T max(T a, T b)
{
	return a < b ? b : a;
}

int main()
{
	// 例程 1 调用
	std::cout << "max(2, 5) = " << max(2, 5) << std::endl;

	return 0;
}

#endif 