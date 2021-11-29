#include <iostream>
#include <vector>
#include <cassert>

/* 类模板初探--以 stack 为例 */

// 这个类的类型是 Stack<T>， 其中 T 是模板参数
template<typename T>
class Stack
{
public:
	Stack()
	{
		std::cout << "-------------- Template Stack ------------" << std::endl;
	}

	// 注意：模板参数只需要提供那些会被用到的操作（而不是可能会被用到的操作）
	void push(const T &elem)
	{
		s.push_back( elem );
	}

	void pop()
	{
		assert( !s.empty() );

		s.pop_back();
	}

	const T& top()
	{
		assert(!s.empty());

		return s.back();
	}

	bool empty() const
	{
		return s.empty();
	}

	void printStack() const
	{
		for (auto it : s) {
			std::cout << it << std::endl;
		}
	}

private:
	// 在类模板内部，T 可以像普通类型一样被用来声明成员变量和成员函数;
	std::vector<T> s;
};

// 情形二：类模板特化:可以对类模板的某一个模板参数进行特化
// 类模板的"特化"允许我们对某一特定类型做优化，或者去修正类模板针对某一特定类型实例化之后的行为;
template<> // 为了特化一个类模板，在类模板声明的前面需要有一个 template<>，并且需要指明所希望特化的类型
class Stack<std::string>
{
public:
	Stack()
	{
		std::cout << "-------------- Template Stack Full Specialization ------------" << std::endl;
	}

	void push(const std::string &elem)
	{
		s.push_back(elem);
	}

	void pop()
	{
		assert(!s.empty());

		s.pop_back();
	}

	const std::string& top()
	{
		assert(!s.empty());

		return s.back();
	}

	bool empty() const
	{
		return s.empty();
	}

	void printStack() const
	{
		for (auto it : s) {
			std::cout << it.c_str() << std::endl;
		}
	}

private:
	// 对于被特化的模板，所有成员函数的定义都应该被定义成“常规”成员函数，也就是说所有 出现 T 的地方，都应该被替换成用于特化类模板的类型
	std::vector<std::string> s; 
};


// 情形三：类模板偏特化（部分特化）:类模板可以只被部分的特例化。这样就可以为某些特殊情况提供特殊的实现，不过使用者还是要定义一部分模板参数
template<typename T> 
class Stack<T*> // 定义了一个依然是被类型 T 参数化，但是被特化用来处理指针的类模板（Stack<T*>）
{
public:

	Stack()
	{
		std::cout << "-------------- Template Stack Partial Specialization ------------" << std::endl;
	}

	void push(T* elem)
	{
		s.push_back(elem);
	}

	void pop()
	{
		assert(!s.empty());

		s.pop_back();
	}

	const T* top()
	{
		assert(!s.empty());

		return s.back();
	}

	bool empty() const
	{
		return s.empty();
	}

	void printStack() const
	{
		for (auto it : s) {
			std::cout << *it << std::endl;
		}
	}

private:
	std::vector<T*> s;
};


int main()
{
	// 直到 C++17，在使用类模板的时候都需要显式的指明模板参数;
	Stack<int> s; // 通过声明 Stack<int>类型，在类模板内部 int 会被用作类型 T;
				  // 被实例化之后的类模板类型（Stack<int>之类）可以像其它常规类型一样使用


	// 注意：模板函数和模板成员函数只有在被调用的时候才会实例化
	for (int i = 0; i < 10; i ++ ){
		s.push(i);
	}
	
	s.printStack();

	Stack<std::string> s1;
	s1.push("hello ");
	s1.push("world ");
	s1.push(" !");

	s1.printStack();

	Stack<int*> s2;
	int i1 = 10;
	int i2 = 20;
	int i3 = 30;
	int i4 = 40;
	s2.push( &i1 );
	s2.push(&i2);
	s2.push(&i3);
	s2.push(&i4);

	s2.printStack();

	return 0;
}
