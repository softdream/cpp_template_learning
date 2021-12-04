#include <iostream>
#include <type_traits>

// SFINAE会将在模板参数推断过程中，构造无效类型和表达式的潜在错误（会导致程序出现语法错误）
// 转换成简单的推断错误，这样就允许重载解析继续在其它待选项中间做选择;
// 虽然 SFINAE 最开始是被用来避免与函数模板重载相关的伪错误，
// 我们也可以用它在编译期间判断特定类型和表达式的有效性；
// 比如我们可以通过萃取来判断一个类型是否有某个特定的成员，是否支持某个特定的操作，或者该类型本身是不是一个类。

// 情形一：用 SFINAE 排除某些重载函数：
// 下面这个例子是将 SFINAE 用于函数重载，以判断一个类型是否是默认可构造的，对于可以默认构造的类型，
// 就可以不通过值初始化来创建对象。也就是说， 对于类型 T，诸如 T()的表达式必须是有效的。

template<typename T>
struct IsDefaultConstructibleT 
{
private: 
	// test() trying substitute call of a default constructor for 
	//T passed as U : 
	template<typename U, typename = decltype(U())> 
	static char test(void*);// test() fallback:
	
	template<typename> 
	static long test( ... ); 

public: 
	// 返回值 value 的具体值取决于最终选择了哪一个 test 函数;
	// 如果选择的是第一个 test()函数，由于其返回值类型是 char，
	// value 会被初始化伟 is_same<char, char>，也就是 true。
	// 否则，value 会被初始化伟 isSame<long, char>，也就是 false。
	static constexpr bool value = std::is_same<decltype(test<T>(nullptr)), char>::value; 
};
// 通过函数重载实现一个基于 SFINAE 的萃取的常规方式是
// 声明两个返回值类型不同的同名 （test()）重载函数模板.
// 第一个重载函数只有在所需的检查成功时才会被匹配到（后文会讨论其实现方式）;
// 第二个 重载函数是用来应急的：它会匹配任意调用，
// 但是由于它是通过”...”（省略号）进行匹配的， 因此其它任何匹配的优先级都比它高;

// 分析：
// 这个例子的目标是只有当我们所关心的测试条件被满足的时候，才可以使第一个 test()有效。
// 我们想要测试的条件是被传递进来的类型 T 是否是可以被默认构造的,
// 为了实现这一目的，我们将 T 传递给 U，并给第一个 test()声明增加一个无名的（dummy）模板参数,
// 该模板参数被一个只有在这一转换有效的情况下才有效的构造函数进行初始化;
// 在这个例子中，我们使用的是只有当存在隐式或者显式的默认构造函数 U()时才有效的表达式。
// 我们对 U()的结果施加了 deltype 操作，这样就可以用其结果初始化一个类型参数了。

// 第二个模板函数参数不可以被推断，因为我们不会为之传递任何参数。
// 而且我们也不会为之提供显式的模板参数。因此，它只会被替换，如果替换失败，
// 基于 SFINAE，相应的 test()声明会被丢弃掉，因此也就只有应急方案可以匹配相应的调用。


// 改进：将基于 SFINAE 的萃取变成预测型萃取；
// 为了这一目的，我们需要间接定义一个 IsDefaultConstructibleT。
// 该萃取本身需要继承自一个辅助类的 Type 成员，该辅助类会返回所需的基类。

struct S
{ 
	S() = delete; // S 的默认构造函数被禁了；
};

int main()
{
	// 我们可以像下面这样使用这一萃取：
	IsDefaultConstructibleT<int>::value;//yields true

	IsDefaultConstructibleT<S>::value; //yields false

	return 0;
}