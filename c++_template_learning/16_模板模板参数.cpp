#include <iostream>
#include <vector>
#include <deque>
#include <cassert>

// 允许模板参数也是一个类模板

// 对 5.5 节中的 stack 模板，如果不想使用默认的内部容器类型 std::deque，那么就需要两次指定 stack 元素的类型。
// 也就是说为了指定内部容器的类型，必须同时指出容器的类型和元素 的类型： 
// Stack<int, std::vector<int>> vStack; // integer stack that uses a vector

// 使用模板参数模板，在声明 Stack 类模板的时候就可以只指定容器的类型而不去指定容器中元素的类型： 
// Stack<int, std::vector> vStack; // integer stack that uses a vector

// 为此就需要在 Stack 的定义中将第二个模板参数声明为模板参数模板。
// template<typename T, typename Container = std::vector<T>> 替换为：
template<typename T, 
template<typename Elem, typename Alloc = std::allocator<Elem>> class Container = std::vector>
// 这里的 Alloc 可以省略不写
class Stack
{
	using Iterator = typename Container<T>::iterator;

public:
	Stack()
	{
		std::cout << "-------------- Template Stack ------------" << std::endl;
	}

	void push(const T &elem)
	{
		s.push_back(elem);
	}

	void pop()
	{
		assert(!s.empty());

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

	// 使用模板重载拷贝赋值函数 同样需要重写
	template<typename T2, template<typename Elem2, 
			 typename Alloc2 = std::allocator<Elem2>> class Container2 >
		// 其中 Alloc2 这个类名字可以省略
	const Stack& operator=(const Stack<T2, Container2> &rhs)
	{
		Stack<T2, Container2> tmp(rhs); // create a copy of the assigned stack 
		s.clear(); // remove existing elements 

		while (!tmp.empty()) { // copy all elements 
			s.push_front(tmp.top());
			tmp.pop();
		}

		return *this;
	}

private:
	Container<T> s;
};


int main()
{
	Stack<int, std::deque> s1;
	s1.push(10);
	s1.push(20);
	s1.push(30);
	s1.printStack();

	Stack<float, std::deque> s2;
	s2 = s1;
	s2.printStack();

	return 0;
}