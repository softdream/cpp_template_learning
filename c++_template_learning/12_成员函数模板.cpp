#include <iostream>
#include <deque>
#include <cassert>

// 类的成员也可以是模板，对嵌套类和成员函数都是这样 -- 以栈为例
// 存在的问题：通常只有当两个 stack 类型相同的时候才可以相互赋值（stack 的类型相同说明它们的元素类型也相同）。
// 即使两个 stack 的元素类型之间可以隐式转换， 也不能相互赋值；
// 解决办法：
// 只要将赋值运算符定义成模板，就可以将两个元素类型可以做转换的 stack 相互赋值
template<typename T>
class Stack
{
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

	// 使用成员模板函数重载拷贝赋值函数
	template<typename T2>
	const Stack& operator=(const Stack<T2> &rhs)
	{
		Stack<T2> tmp(rhs); // create a copy of the assigned stack 
		s.clear(); // remove existing elements 
		
		while (!tmp.empty()) { // copy all elements 
			s.push_front(tmp.top()); 
			tmp.pop(); 
		}
		
		return *this;
	}

private:
	std::deque<T> s;
};

int main()
{
	Stack<int> intStack;
	intStack.push(10);
	intStack.push(20);
	intStack.push(30);
	Stack<float> floatStack;

	floatStack = intStack; // OK: stacks have different types, 
						   // but int converts to float
	// 这样的赋值就不会改变 floatStack 的类型，也不会改变它的元素的类型。
	// 在赋值之后， floatStack 存储的元素依然是 float 类型，top()返回的值也依然是 float 类型。

	floatStack.printStack();

	return 0;
}