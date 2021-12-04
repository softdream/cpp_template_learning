#include <iostream>
#include <list>
#include <vector>

// 假设我们有很多的容器模板，比如 std::vector<>和 std::list<>，也可以包含内置数组。
// 我们希望得到这样一个类型函数，当给的一个容器类型时，它可以返回相应的元素类型。
// 这可以通过偏特化实现:
// 偏特化的使用使得我们可以在容器类型不知道具体类型函数存在的情况下去实现类型函数;

template<typename T> 
struct ElementT; // primary template 

template<typename T>
struct ElementT<std::vector<T>> 
{ 
	//partial specialization for std::vector 
	using Type = T; 
};

template<typename T> 
struct ElementT<std::list<T>> 
{ 
	//partial specialization for std::list 
	using Type = T; 
};

template<typename T, std::size_t N> 
struct ElementT<T[N]> 
{ 
	//partial specialization for arrays of known bounds
	using Type = T;
};

template<typename T> 
struct ElementT<T[]> 
{ 
	//partial specialization for arrays of unknown bounds 
	using Type = T; 
};

template<typename T>
void printElementType(T const& c) 
{ 
	std::cout << "Container of " << typeid(typename ElementT<T>::Type).name() << " elements."<<std::endl;
}

// 偏特化的使用使得我们可以在容器类型不知道具体类型函数存在的情况下去实现类型函数。 
// 但是在某些情况下，类型函数是和其所适用的类型一起被设计的，此时相关实现就可以被简化。

template<typename C> 
struct ElementT
{
	using Type = typename C::value_type;
};
template<typename C> 
ElementType<C> sumOfElements(C const& c);

int main() 
{ 
	std::vector<bool> s; 
	printElementType(s);

	int arr[42]; 
	printElementType(arr);

	return 0;
}