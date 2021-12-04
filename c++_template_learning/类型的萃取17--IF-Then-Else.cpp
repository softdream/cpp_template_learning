#include <iostream>
#include <type_traits>

// 我们可以用一个特殊的类型模板 IfThenElse 来表达这一 if-then-else 的行为，
// 它接受一个 bool 型的模板参数，并根据该参数从另外两个类型参数中间做选择;

// primary template: yield the second argument by default and rely on 
// a partial specialization to yield the third argument 
// if COND is false 
template<bool COND, typename TrueType, typename FalseType> 
struct IfThenElseT 
{ 
	using Type = TrueType;
};

// partial specialization: false yields third argument 
template<typename TrueType, typename FalseType> 
struct IfThenElseT<false, TrueType, FalseType> 
{
	using Type = FalseType;
};

template<bool COND, typename TrueType, typename FalseType>
using IfThenElse = typename IfThenElseT<COND, TrueType, FalseType>::Type;

// 例：
template<auto N> 
struct SmallestIntT
{
	typename IfThenElseT<N <= std::numeric_limits<char>::max(), char,
		typename IfThenElseT<N <= std::numeric_limits<short>::max(), short, 
			typename IfThenElseT<N <= std::numeric_limits<int>::max(), int, 
				typename IfThenElseT<N <= std::numeric_limits<long>::max(), long, 
					typename IfThenElseT<N <= std::numeric_limits<long long>::max(), long long, //then void //fallback 
					>::Type 
				>::Type 
			>::Type 
		>::Type 
	>::Type;
};

// 在 C++标准库中有与 IfThenElseT 模板对应的模板（std::conditional<>）;

int main()
{
	SmallestIntT<10>();

	return 0;
}