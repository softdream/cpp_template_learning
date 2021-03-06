#include <iostream>

// 移动语义（move semantics）是 C++11 引入的一个重要特性。
// 在 copy 或者赋值的时候，可以通过它将源对象中的内部资源 move（“steal”）到目标对象，
// 而不是 copy 这些内容。当然这样做的前提是源对象不在需要这些内部资源或者状态（因为源对象将会被丢弃）;

// 移动构造函数和移动赋值运算符也给予人们控制对象移动语义的能力，移动语义也使得创建 ‘只移型别对象’成为可能，
// 这些型别包括：std::unique_ptr, std::future, std::thread等。

// 完美转发：
// 完美转发使得人们可以撰写接受任意实参的函数模板，并将其转发到其它函数，
// 目标函数会接受到与转发函数所接受的完全相同的实参。

// 右值引用是将上述两种完全不同的语言特性胶合在一起的底层语言机制，正是它使得移动语义和完美转发成为了可能。

// 注意一：形参总是左值，即使其型别是右值引用。例如：
// void f( Widget&& w );
// 形参 w  是一个左值，即使它的型别是个指涉到 Widget 型别对象的右值引用。

// 注意二：std::move并不进行任何移动，std::forward并不进行任何转发；
// std::move 和 std::forward 都是仅仅执行强制型别转换的函数（模板函数）；
// std::move 无条件的将实参强制转换成左值，std::forward 则仅仅在满足某个特定条件的条件下才执行同一个型别转换；

// 注意三：1. 如果想取得对某个对象执行移动操作的能力，则不要将其声明为常量，因为针对常量对象执行的移动操作将会转为赋值操作。
// 2. std::move 不仅不实际移动任何东西，甚至不保证经过其强制类型转换后的对象具有可移动的能力。

// 注意四：std::forward 仅当其实参是使用右值完成初始化时，它才会执行向右 值型别的强制型别转换。

int main()
{

	return 0;
}