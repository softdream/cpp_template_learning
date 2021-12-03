#include <iostream>

// 模板允许我们用多种类型对类和函数进行参数化;
// 但是从实际的角度来看， 我们并不希望引入过多的模板参数来实现最大化的参数化;
// 幸运的是，时间证明大部分被引入的额外的模板参数都有合理的默认值。在一些情况下，额 外的模板参数可以完全由很少的、主要的模板参数决定;
// 在大多数情况下，其它一些参数的默认值可以从主模板参数得 到，但是默认值需要偶尔被重载（针对特殊应用）。

//萃取（或者叫萃取模板，traits/traits template）是 C++编程的组件，它们对管理那些在设计工业级应用模板时所需要管理的多余参数很有帮助;

// 固定萃取--Fixed Traits
// 例一：待求和的数据存储在一个数组中，然后我们有一个指向数组中第一个元素的指针，和一个指向最后一个元素的指针，返回总和；
template<typename T> 
T accum(T const* beg, T const* end)
{
	// 零初始化
	T total{}; // assume this actually creates a zero value 

	// 遍历相加即可
	while (beg != end) {
		total += *beg; 
		++beg;
	}
	return total; 
}

// 一个可以避免使用额外的模板参数的方式是，在每个被用来实例化 accum()的 T 和
// 与之对应的应该被用来存储返回值的类型之间建立某种联系。这一联系可以被认为是 T 的某种属性。 
// 正如下面所展示的一样，可以通过模板的偏特化建立这种联系;
template<typename T>
struct AccumulateTrait;

template<>
struct AccumulateTrait<char>
{
	using AccT = int;
};

template<>
struct AccumulateTrait<short>
{
	using AccT = int;
};

template<>
struct AccumulateTrait<int>
{
	using AccT = long;
};

template<>
struct AccumulateTrait<unsigned int>
{
	using AccT = unsigned long;
};

template<>
struct AccumulateTrait<float>
{
	using AccT = double;
};

// AccumulationTraits 模板被称为萃取模板，因为它是提取了其参数类型的特性。
// （通常而言可 以有不只一个萃取，也可以有不只一个参数）;

template<typename T> 
auto accum1(T const* beg, T const* end)
{ 
	// return type is traits of the element type 
	using AccT = typename AccumulateTrait<T>::AccT;

	AccT total{}; // assume this actually creates a zero value
	while (beg != end) { 
		total += *beg; 
		++beg; 
	}
	
	return total; 
}

int main()
{
	// 例一：
	int num[] = { 1, 2, 3, 4, 5 };
	std::cout << "accum = " << accum( num, num + 5 )<<std::endl; // OK，accum = 15;

	char name[] = "hello world";
	int length = sizeof(name) - 1;
	std::cout << "accum = " << accum(name, name + length) << std::endl; // OK，accum = '\';

	std::cout << "accum = " << accum1(name, name + length) << std::endl; // OK, accum = 1116;
 
	return 0;
}
