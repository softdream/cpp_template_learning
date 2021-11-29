#include <iostream>
#include <vector>
#include <cassert>
#include <deque>

/* 类模板默认参数和类型别名以及别名模板（模板别名，Alias Templates）--以 stack 为例 */

// 和函数模板一样，也可以给类模板的模板参数指定默认值
template<typename T, typename Container = std::vector<T>>
class Stack
{
	using Iterator = typename Container::iterator; 

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

private:

	//std::vector<T> s; // 替换成：
	Container s;
};


// 要点：通过给类模板定义一个新的名字，可以使类模板的使用变得更方便
// 方法一：使用 typedef 关键字
typedef Stack<int> IntStack; // 我们称这种声明方式为 typedef， 被定义的名字叫做 typedef-name

// 方法二：使用 using 关键字（c++11）
using DoubleStack = Stack<double>; // 这一过程叫做 alias declaration

// 以上两种给一个已经存在的类型定义新名字的方式，被称为 type alias declaration。新的名字被称为 type alias;

// 别名模板情景一：不同于 typedef， alias declaration 也可以被模板化，这样就可以给一组类型取一个方便的名字。
// 这一特性从 C++11 开始生效，被称作 alias templates（别名模板）。
template<typename T> 
using DequeStack = Stack<T, std::deque<T>>; // DequeStack 是 模板Stack<T, std::deque<T>>的一个别名，元素类型 T 依然未知。
											// 只是换了一个皮，注意和类型别名区分开来。
// DequeStack<int>和 Stack<int, std::deque<int>> 代表的是同一种类型。

// 使用 alias templates 可以很方便的给类模板的成员类型定义一个快捷方式

// 别名模板情景二：使用 alias templates 可以很方便的给类模板的成员类型定义一个快捷方式
template<typename T>
using StackIterator = typename Stack<T>::Iterator; // 注意这里要加 typename

// 例如在 容器 std::vector<T>中：
template<typename T>
using VectorIterator = typename std::vector<T>::iterator;

int main()
{
	
	Stack<int> s; 

	for (int i = 0; i < 10; i++) {
		s.push(i);
	}

	s.printStack();
	
	// stack of doubles using a std::deque<> to manage the elements
	Stack<double, std::deque<double>> s2;

	for (double i = 10; i < 100; i += 10.0) {
		s2.push( i );
	}
	s2.printStack();

	std::cout << "type of IntStack : " << typeid(IntStack).name() << std::endl;
	std::cout << "type of DoubleStack : " << typeid(DoubleStack).name() << std::endl;

	std::vector<int> vec = { 1, 2, 3, 4, 5, 6 };
	VectorIterator<int> iter = vec.begin(); // 这里 VectorIterator 就是 std::vector<T>::iterator的一个别名；
	for (; iter != vec.end(); iter++) {
		std::cout << *iter << std::endl;
	}

	return 0;
}
