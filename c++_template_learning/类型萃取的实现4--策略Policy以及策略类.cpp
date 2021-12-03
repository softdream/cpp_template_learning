#include <iostream>

// 到目前为止我们并没有区分累积（accumulation）和求和（summation）。但是我们也可以想像其它种类的累积。
// 比如，我们可以对一组数值求积。或者说，如果这些值是字符串的话， 我们可以将它们连接起来。
// 即使是求一个序列中最大值的问题，也可以转化成一个累积问题。 
// 在所有这些例子中，唯一需要变得的操作是 accum()中的 total += *beg。
// 我们可以称这一操作为累积操作的一个策略（policy）；

template<typename T>
struct AccumulateTrait;

template<>
struct AccumulateTrait<char>
{
	using AccT = int;
	static constexpr AccT zero()
	{
		return 0;
	}
};

template<>
struct AccumulateTrait<short>
{
	using AccT = int;
	static constexpr AccT zero()
	{
		return 0;
	}
};

template<>
struct AccumulateTrait<int>
{
	using AccT = long;
	static constexpr AccT zero()
	{
		return 0;
	}
};

template<>
struct AccumulateTrait<unsigned int>
{
	using AccT = unsigned long;
	static constexpr AccT zero()
	{
		return 0;
	}
};

template<>
struct AccumulateTrait<float>
{
	using AccT = double;

	static constexpr AccT zero()
	{
		return 0;
	}
};

template<typename T, typename Policy = SumPolicy, typename Traits = AccumulateTrait<T>>
auto accum(T const* beg, T const* end) {
	using AccT = typename Traits::AccT;
	AccT total = Traits::zero(); 
	while (beg != end) { 
		Policy::accumulate(total, *beg); 
		++beg;
	}
	return total;
}

// 在这一版的 accum()中，SumPolicy 是一个策略类，也就是一个通过预先商定好的接口，为算法实现了一个或多个策略的类。
// SumPolicy 可以被实现成下面这样
class SumPolicy {
public: 
	template<typename T1, typename T2> 
	static void accumulate(T1& total, T2 const& value) { 
		total += value; 
	} 
};

// 如果提供一个不同的策略对数值进行累积的话，我们可以计算完全不同的事情。
// 比如考虑下面这个程序，它试图计算一组数值的乘积：
class MultPolicy
{ 
public:
	template<typename T1, typename T2>
	static void accumulate(T1& total, T2 const& value) {
		total *= value; 
	}
};

// 要点一：我们倾向于对策略类这一名词的使用做如下限制：它们应该是一些编码了某种与其 它模板参数大致独立的行为的类；
// 策略和萃取有很多相似之处，只是它们更侧重于行为，而不是类型。
// 要点二： 萃取类：一个用来代替模板参数的类。作为一个类，它整合了有用的类型和常量；
// 作为一个模板，它为实现一个可以解决所有软件问题的“额外的中间层”提供了方法。

int main() 
{ 
	// create array of 5 integer values 
	int num[] = { 1, 2, 3, 4, 5 }; 
	// print product of all values 
	std::cout << "the product of the integer values is " << accum<int,MultPolicy>(num, num+5) << std::endl; 
	// 打印结果是：the product of the integer values is 0
	// 这是不对的。问题出在我们对初始值的选取：虽然 0 能很好的满足求和的需求，但是却不适用于求乘积（初始值 0 会让乘积的结果也是 0）。
	// 这说明不同的萃取和策略可能会相互影响。


	return 0;
}
