#include <iostream>

// 为了实现累积策略（accumulation policy），我们选择将 SumPolicy 和 MultPolicy 实现为有成员模板的常规类。
// 另一种使用类模板设计策略类接口的方式，此时就可以被当作模板模板参数使用（template template arguments）;
// 我们可以将 SumPolicy 重写为如下模板：

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

template<typename T1, typename T2>
class SumPolicy 
{ 
public: 
	static void accumulate(T1& total, T2 const& value) 
	{ 
		total += value; 
	}
};

// 此时就可以调整 Accum，让其使用一个模板模板参数:
// 通过模板模板参数访问策略类的主要优势是，让一个策略类通过一个依赖于模板参数的类型
// 携带一些状态信息会更容易一些（比如 static 数据成员）;
// 模板模板参数方法的一个缺点是，策略类必须被实现为模板，而且模板参数必须和我们的接口所定义的参数一样。
// 这可能会使萃取本身的表达相比于非模板类变得更繁琐，也更不自然。
template<typename T, 
		 template<typename, typename> 
		 class Policy = SumPolicy, 
		 typename Traits = AccumulateTrait<T>>
auto accum(T const* beg, T const* end)
{
	using AccT = typename Traits::AccT;
	AccT total = Traits::zero();

	while (beg != end) {
		Policy<AccT, T>::accumulate(total, *beg);
		++beg;
	}

	return total;
}

int main()
{
	// create array of 5 integer values 
	int num[] = { 1, 2, 3, 4, 5 };
	// print product of all values 
	std::cout << "the product of the integer values is " << accum<int, SumPolicy>(num, num + 5) << std::endl;
	
	return 0;
}