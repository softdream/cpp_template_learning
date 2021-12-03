#include <iostream>

// 在前面几节中，在 accum()里使用的萃取被称为固定的（fixed），这是因为一旦定义了解耦合萃取，
// 在算法中它就不可以被替换。但是在某些情况下，这一类重写（overriding）行为却又是我们所期望的。
// 比如，我们可能碰巧知道某一组 float 数值的和可以被安全地存储在 一个 float 变量中，
// 而这样做可能又会带来一些性能的提升。

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


// 为了解决这一问题，可以为萃取引入一个新的模板参数 AT，其默认值由萃取模板决定:
template<typename T, typename AT = AccumulateTrait<T>>
auto accum(T const* beg, T const* end)
{
	typename AT::AccT total = AT::zero();

	while (beg != end) {
		total += *beg;
		++beg;
	}

	return total;
}

// 采用这种方式，一部分用户可以忽略掉额外模板参数，而对于那些有着特殊需求的用户，他 们可以指定一个新的类型来取代默认类型;

int main()
{
	int a[3] = { 1, 2, 3 };
	std::cout << "accum = " << accum(a, a + 3) << std::endl; // OK，accum = 6;

	return 0;

}
