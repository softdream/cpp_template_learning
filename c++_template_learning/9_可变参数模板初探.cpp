#include <iostream>

// 可变参数模板：从 C++11 开始，模板可以接受一组数量可变的参数。这样就可以在参数数量和参数类型都不确定的情况下使用模板；

// 例一：变参函数模板
// 为了结束递归，重载了不接受参数的非模板函数 print()，它会在参数包为空的时候被调用;
void print()  // 递归退出的条件
{
	std::cout << std::endl;
}

// typename... Types 是模板参数包（template parameter pack）
template<typename T, typename... Types>
void print(T firstArg, Types... args) {
	// ，这里通过将第一个参数单独声明， 就可以先打印第一个参数，然后再递归的调用 print()来打印剩余的参数
	std::cout << firstArg << ", "; //print first argument 

	// C++11 为变参模板引入了一种新的 sizeof 运算符：sizeof...。它会被扩展成参数包中所包含的参数数目。
	std::cout << "sizeof...(args): " << sizeof...(args) << std::endl;

	// 这些被称为 args 的剩余参数，是一个函数参数包（function parameter pack）
	print(args...); // call print() for remaining arguments 
}

// 例一也可以写成：
template<typename T>
void print2( T arg )
{
	std::cout << arg << ", ";
}

template<typename T, typename... Types>
void print2(T firstArg, Types... args)
{
	print2( firstArg );
	print2( args... ); // ，当两个函数模板的区别只在于尾部的参数包的时候，会优先选择没有尾部参数包的那一个函数模板
	std::cout << std::endl;
}

// 要点一：可变参数模板的一个重要的作用是转发任意类型和数量的参数。
// 要点二：除了转发所有参数之外，还可以做些别的事情。比如计算它们的值。

// 可变参数模板也支持类模板，比如std::tuple
template<typename... Values> 
class tuple; // 类的申明

template<>
class tuple<> // 一个空模板参数的类作为递归结束条件
{

};

// tuple类继承除首之外的其他参数的子tuple类，以此类推，最终继承空参数的tuple，这个是递归继承方式；
// 考虑在内存中的分布，内存中先存储父类的变量成员，再保存子类的变量成员;
template<typename Head, typename... Tail>
class tuple<Head, Tail...> : private tuple<Tail...> // 解析模板参数包是通过继承来实现的
{
	typedef tuple<Tail...> inherited;
public:
	tuple() 
	{
	
	}

	tuple(Head v, Tail... vtail) : m_head(v), inherited(vtail...) 
	{
	
	}

	Head& head() { 
		return m_head; 
	}

	// tail()函数返回的是父类对象，父类对象和子类对象的内存起始地址其实是一样的，因此返回*this，再强行转化为inherited类型
	inherited& tail() { 
		return *this; 
	}

protected:
	Head m_head;
};


int main()
{
	print( 20, 19.8, 9.8 );
	print2(20, 19.8, 9.8);

	std::string s = "hello";
	tuple<int, float, std::string> t(1, 2.3, s);
	std::cout << t.head() << " " << t.tail().head()  << std::endl;


	return 0;
}