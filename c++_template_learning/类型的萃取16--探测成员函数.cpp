#include <iostream>
#include <type_traits> // for true_type, false_type, and void_t
#include <vector>

// primary template: 
template<typename, typename = std::void_t<>> 
struct HasBeginT : std::false_type 
{

};

// partial specialization (may be SFINAE’d away): 
template<typename T> 
struct HasBeginT<T, std::void_t<decltype(std::declval<T> ().begin())>> : std::true_type 
// 这里我们使用 decltype(std::declval<T> ().begin())来测试是否能够调用 T 的 begin()。
{

};

int main()
{
	std::cout << HasBeginT<std::vector<int>>::value << std::endl;

	return 0;
}