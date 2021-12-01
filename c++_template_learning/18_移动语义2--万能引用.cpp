#include <iostream>
#include <vector>

// 欲声明指涉到某型别 T 的右值引用，就写作 “T &&”;“T &&”有两种含义：
// 第一种含义，理所当然就是 右值引用；
// 第二种含义, 则表示其既可以是右值引用，亦可以是左值引用，二者居一；带有这种含义的引用在代码中形如右值引用（T &&）,
// 但它们可以像左值引用一样运作（T &）。这种双重特性使之既可以绑定到右值，也可以绑定到左值。
// 犹有进者，他们也可以绑定到 const 对象或者 非const 对象，以及 volatile 对象或者 非volatile 对象，甚至绑定到既带有
// const 又带有 volatile 修饰的对象，它们几乎可以绑定到任何类型的对象，这种灵活的引用称之为--万能引用（universal reference）；

// 万能引用的使用场景：
// 1. 作为函数模板的形参，例：
template<typename T>
void f(T &&param) // 注意这里加 const 就不是万能引用了，而变成了 右值引用。
{
	std::cout << "typeof param: " << typeid(param).name() << std::endl;
}

// 2. 第二个场景是 auto 声明，例：
// auto&& var2 = var1;

// 这两种情况的共同点都是涉及到型别推导。
// 若要使一个引用成为万能引用，其涉及到型别推导是必要条件，而不是充分条件。

int main()
{
	int a = 10;
	f( a );

	f(std::move(a));

	auto&& var = a;
	std::cout << "typeof var: " << typeid(var).name() << std::endl;

	std::vector<int> vec;
	vec.emplace_back(10);
	// 以 vector 容器的 emplace_back 方法为例，下方是它的实现源码：
	// template<class... _Valty>
	//	void emplace_back(_Valty&&... _Val)
	//	{	// insert bool at end
	//		bool _Tmp(_STD forward<_Valty>(_Val)...);
	//		push_back(_Tmp);
	//	}
	// 可以看出 emplace_back 用了 variadic template，实实在在的涉及到型别推导，因此这里的 _Valty&&... 就是一个万能引用。

	return 0;
}