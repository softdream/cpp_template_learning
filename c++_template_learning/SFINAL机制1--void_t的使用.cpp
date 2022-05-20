#include <iostream>
#include <type_traits>

/*
std::voit_t 功能：
	能够检测到应用SFINAL机制时出现的非法类型，即，模板中进来的类型必须是一个有效的类型。
std::voit_t应用：
	1. 判断类中是否有某个类型别名；
	2. 判断类中是否存在某个成员变量；
	3. 判断类中是否存在某个成员函数；
*/

// 1. 判断类中是否有某个类型别名；
struct NoInerType
{

};

struct HaveInerType
{
	int m_i;
	using type = int; // 某个类型别名

	void myfunc()
	{

	}
};

template<typename T, typename U = std::void_t<>>
struct HasTypeMember : std::false_type
{

};

template<typename T>
struct HasTypeMember<T, std::void_t<typename T::type>> : std::true_type
{

};

// 2. 判断类中是否存在某个成员变量
template<typename T, typename U = std::void_t<>>
struct HasMemberVariable 
{
	const static bool value = false;
};

template<typename T>
struct HasMemberVariable<T, std::void_t<typename decltype(T::m_i)>> 
{
	const static bool value = true;
};

// 3. 判断类中是否存在某个成员函数；
template<typename T, typename U = std::void_t<>>
struct HasMemberFunction : std::false_type
{

};

template<typename T>
struct HasMemberFunction<T, std::void_t<decltype(std::declval<T>().myfunc())>> : std::true_type
{

};

int main()
{
	// HaveTypeMember<NoInerType> 在面对特化版本时，由于没有类型type，即T::type为非法类型，
	// 所以忽略特化版本，从而实例化泛化版本，泛化版本继承std::false_type,有成员value值为0；
	std::cout << HasTypeMember<NoInerType>::value << std::endl;

	//  HaveTypeMember<HaveInerType> 在面对特化版本时，有成员类型type，
	// 从而std::voit_t<typename T::type> 检测不是非法类型，其值为void，从而实例化特化版本，
	// 父类std::true_type有value = 1；
	std::cout << HasTypeMember<HaveInerType>::value << std::endl;

	// 原理同上，注意delctype能用于静态类型推导
	std::cout << HasMemberVariable<NoInerType>::value << std::endl;

	std::cout << HasMemberVariable<HaveInerType>::value << std::endl;

	// 注意std::declval用法
	std::cout << HasMemberFunction<NoInerType>::value << std::endl;
	std::cout << HasMemberFunction<HaveInerType>::value << std::endl;

	return 0;
}