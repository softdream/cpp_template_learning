#include <iostream>
#include <string>
#include <type_traits>

// 我们要解决的问题是：当传递的模板参数的类型不正确的时候（比如不是 std::string 或者可 以转换成 std::string 的类型），禁用如下构造函数模板;

// 例：
class Person 
{
	template<typename T>
	using EnableIfString = std::enable_if_t<std::is_convertible<T, std::string>::value>;

	// 除了使用要求类型之间可以隐式转换的 std::is_convertible<> 之外,还可以使用 std::is_constructible<>，
	// 它要求可以用显式转换来做初始化。但是需要注意的是,它的参数顺序和 std::is_convertible<>相反： 
	// template<typename T> 
	// using EnableIfString = std::enable_if_t<std::is_constructible<std::string, T>::value>;

public: 
	// generic constructor for passed initial name: 
	template<typename STR, typename = EnableIfString<STR>> 
	// 注意这里 STR &&n 是个万能引用；
	explicit Person(STR&& n) : name(std::forward<STR>(n)) 
	{ 
		std::cout << "TMPL-CONSTR for ’" << name << "’\n"; 
	}
	
	// copy constructor: 
	Person (Person const& p) : name(p.name) 
	{ 
		std::cout << "COPY-CONSTR Person ’" << name << "’\n"; 
	}
	
	// move constructor
	Person (Person&& p) : name(std::move(p.name)) 
	{ 
		std::cout << "MOVE-CONSTR Person ’" << name << "’\n"; 
	} 

private:
	std::string name;
};

// 注意我们不能通过使用 enable_if<>来禁用 copy/move 构造函数以及赋值构造函数。
// 这是因为成员函数模板不会被算作特殊成员函数（依然会生成默认构造函数），
// 而且在需要使用 copy 构造函数的地方，相应的成员函数模板会被忽略掉。

// 例如： 
class C 
{
public: 
	C()
	{
		std::cout << "default constructor" << std::endl;
	}

	// 模板拷贝构造函数
	template<typename T>
	C(T const& rhs) 
	{
		std::cout << "tmpl copy constructor" << std::endl;
	}
};

int main()
{
	std::string s = "sname";
	Person p1(s); // init with string object => calls TMPL-CONSTR

	Person p2("tmp"); // init with string literal => calls TMPL-CONSTR

	Person p3(p1); // OK => calls COPY-CONSTR 
	
	// std::move 将左值 p1 转化为右值，这里会调用 移动构造函数
	Person p4(std::move(p1)); // OK => calls MOVE-CONST
	
	C c;
	C d(c); // 这里永远也不会调用到模板拷贝构造函数；

	return 0;
}