#include <iostream>

// 成员函数模板也可以被全部或者部分地特例化；

class BoolString {
private:
	std::string value;
public: 
	BoolString(std::string const& s) : value(s) 
	{
	
	}
	
	template<typename T = std::string>
	T get() const { // get value (converted to T)
		std::cout << "default template function " << std::endl;
		return value; 
	}

	template<>
	bool get<bool>() const { // 全特化
		std::cout << "Full Specifilized Template Function " << std::endl;
		return value == "true" || value == "1" || value == "on";
	}
};

int main()
{
	BoolString a("true");

	std::cout << a.get<bool>() << std::endl; // 这里调用的是特化版本

	BoolString b("hello");
	std::cout << b.get().c_str() << std::endl; // 这里调用的是普通版本

	return 0;
}