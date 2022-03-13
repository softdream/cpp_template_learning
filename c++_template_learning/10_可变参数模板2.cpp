#include <iostream>

template<typename ...Args>
class Test
{
public:
	Test()
	{
		std::cout << "Test 泛化版本 ... this : " << this <<std::endl;
	}
};

template<typename First, typename ...Others>
class Test<First, Others...> : private Test<Others...>
{
public:
	Test()
	{
		std::cout << "-------------------------------------" << std::endl;
		std::cout << "Test 偏特化版本 ... this : " << this << std::endl;
		std::cout << "type of First = " << typeid(First).name() << std::endl;
	}
};

template<>
class Base<>
{

};

template <typename ...Args>
class Base
{
public:
	Base()
	{
		std::cout << "Base 泛化版本 ... this : " << this << std::endl;
	}
};

template <typename First, typename ...Others>
class Base<First, Others ...> : private Base<Others ...>
{
public:
	Base(const First &value, const Others... args) : value_(value), Base<Others ...>(args ...)
	{
		std::cout << "-------------- Base 偏特化版本 ---------------" << std::endl;
		std::cout << "地址：" << this << std::endl;
		std::cout << "value 的地址：" << &this->value_ << std::endl;
		std::cout << "type of value = " << typeid(value_).name() << std::endl;
		std::cout << "value : " << value_ << std::endl;
		std::cout << "----------------------------------------------" << std::endl;
	}

private:
	First value_ = 0;
};

int main()
{
	//Test<int, float, double> t;
	Base<int, float, double> t(30, 40.1f, 50.3);
	std::cout << "对象的地址：" << &t << std::endl;

	return 0;
}

