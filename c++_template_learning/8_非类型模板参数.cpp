#include <iostream>
#include <cassert>

// 对于函数模板和类模板，其模板参数不一定非得是某种具体的类型，也可以是常规数值；
// 对非类型模板参数，待定的不再是类型，而是某个数值

// 例：以栈为例
template<typename T, int Size = 10> // 第二个新的模板参数 Size 是 int 类型的,对非类型模板参数，也可以指定默认值;
class Stack
{
public:
	void push(const T &elem)
	{
		assert( nums < Size );

		s[nums] = elem;
		nums++;
	}

	void pop()
	{
		assert( !isEmpty() );
		nums--;
	}

	const T& top()
	{
		assert(!isEmpty());
		return s[ nums - 1 ];
	}

	bool isEmpty() const
	{
		return nums == 0;
	}

	const int size() const
	{
		return nums;
	}

	void printStack() const
	{
		for (int i = 0; i < nums; i++) {
			std::cout << s[i] << std::endl;
		}
	}

private:
	int nums = 0;
	T s[Size];
};

// 要点一：使用非类型模板参数是有限制的。通常它们只能是整形常量（包含枚举），指向 objects/functions/members 的指针，objects 或者 functions 的左值引用，或者是 std::nullptr_t （类型是 nullptr）；
// 要点二：浮点型数值或者 class 类型的对象都不能作为非类型模板参数使用;
// 要点三：当传递对象的指针或者引用作为模板参数时，对象不能是字符串常量，临时变量或者数据成 员以及其它子对象

int main()
{
	// 为了使用这个类模板，需要同时指出 Stack 中元素的类型和 Stack 的最大容量
	Stack<int, 10> stack;
	for (int i = 10; i <= 50; i += 10) {
		stack.push( i );
	}
	stack.printStack();

	return 0;
}