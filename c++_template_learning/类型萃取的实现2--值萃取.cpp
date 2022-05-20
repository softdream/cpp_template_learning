#include <iostream>

// 到目前为止我们看到的萃取，代表的都是特定“主”类型的额外的类型信息。
// 实际上这一“额外的信息”并不仅限于类型信息，还可以将常量以及其它数值类和一个类型关联起来。

// 在最原始的 accum()模板中，我们使用默认构造函数对返回值进行了初始化，
// 希望将其初始化为一个类似零（zero like）的值： 
// AccT total {}; // assume this actually creates a zero value 
// …
// return total;
// 很显然，这并不能保证一定会生成一个合适的初始值, 因为 AccT 可能根本就没有默认构造函数；

// 我们可以为 AccumulateTraits 添加一个新的值萃取（value trait）来解决这个问题；
template<typename T>
struct AccumulateTrait;

template<>
struct AccumulateTrait<char>
{
	using AccT = int;
	static AccT const zero = 0;
};

template<>
struct AccumulateTrait<short>
{
	using AccT = int;
	static AccT const zero = 0;
};

template<>
struct AccumulateTrait<int>
{
	using AccT = long;
	static AccT const zero = 0;
};

template<>
struct AccumulateTrait<unsigned int>
{
	using AccT = unsigned long;
	static AccT const zero = 0;
};

template<>
struct AccumulateTrait<float>
{
	using AccT = double;
	// 这样做的缺点是 C++ 只允许我们在类中对一个整形或者枚举类型的 static const 数据成员进行初始化；
	//static AccT const zero = 0; // 这句话会报错；
	
	// constexpr 的 static 数据成员会稍微好一些，允许我们对 float 类型以及其它字面值类型进行类内初始化;
	constexpr static AccT zero = 0.0;
};

// 上述新的萃取提供了一个可以在编译期间计算的，const的zero成员;

template<typename T>
auto accum(T const* beg, T const* end)
{
	// return type is traits of the element type 
	using AccT = typename AccumulateTrait<T>::AccT;

	AccT total = AccumulateTrait<T>::zero; // 值萃取；

	while (beg != end) {
		total += *beg;
		++beg;
	}

	return total;
}

// 上面这种值萃取的写法存在的问题是：无论是 const 还是 constexpr 都禁止对非字面值类型进行这一类初始化；
// 例如：有一个自定义类型：
class MyType
{
public:
	MyType() : a(0), b(0), c(0)
	{

	}

	MyType( const int a_, const int b_, const int c_ ) : a(a_), b(b_), c(c_)
	{

	}

	MyType(const std::initializer_list<int>& tmp)
	{
		std::cout << "initializer_list constructor ..." << std::endl;
		a = *tmp.begin();
		b = *(tmp.begin() + 1);
		c = *(tmp.begin() + 2);
	}

	const MyType operator+=( const MyType& x) const
	{
		std::cout << "operator overload ..." << std::endl;
		return MyType(x.a + a, x.b + b, x.c + c);
	}

	void print() const
	{
		std::cout << "a = " << a << std::endl;
		std::cout << "b = " << b << std::endl;
		std::cout << "c = " << c << std::endl;
	}

private:
	int a;
	int b;
	int c;
};

// 为这个类型实现一个 值萃取：
template<>
struct AccumulateTrait<MyType>
{
	using AccT =  MyType;

	//constexpr static MyType zero = MyType{0, 0, 0};// ERROR: not a literal type ,构造函数不是 constexpr 的

	// 类里面进行声明
	static const MyType zero;
};
// 一个比较直接的解决方案是，不再类中定义值萃取（只做声明）;
// 然后在类外对其进行初始化：
MyType const AccumulateTrait<MyType>::zero = MyType{0, 0, 0};
// 这样虽然可以工作，但是却有些麻烦（必须在两个地方同时修改代码），这样可能还会有些低效，因为编译期通常并不知晓在其它文件中的变量定义;


// 在 C++17 之前的另一种解决办法是，对于那些不是总是生成整型值的值萃取，使用 inline 成员函数。
// 我们可以像下面这样重写 AccumulateTraits：

template<typename T>
struct AccumulateTrait2;

template<>
struct AccumulateTrait2<char>
{
	using AccT = int;
	static constexpr AccT zero()
	{
		return 0;
	}
};

template<>
struct AccumulateTrait2<short>
{
	using AccT = int;
	static constexpr AccT zero()
	{
		return 0;
	}
};

template<>
struct AccumulateTrait2<int>
{
	using AccT = long;
	static constexpr AccT zero()
	{
		return 0;
	}
};

template<>
struct AccumulateTrait2<unsigned int>
{
	using AccT = unsigned long;
	static constexpr AccT zero()
	{
		return 0;
	}
};

template<>
struct AccumulateTrait2<float>
{
	using AccT = double;

	static constexpr AccT zero()
	{
		return 0;
	}
};

template<>
struct AccumulateTrait2<MyType>
{
	using AccT = MyType;

	static const AccT zero() // 注意这里不能用 constexpr 关键字；
	{
		return AccT{ 0, 0, 0 };
	}
};

// accum 函数可改为：
template<typename T>
auto accum2(T const* beg, T const* end)
{
	// return type is traits of the element type 
	using AccT = typename AccumulateTrait2<T>::AccT;

	//AccT total = AccumulateTrait<T>::zero; // 值萃取；
	AccT total = AccumulateTrait2<T>::zero(); // 这里换成函数

	while (beg != end) {
		total += *beg;
		++beg;
	}

	return total;
}

int main()
{

	char name[] = "hello world";
	int length = sizeof(name) - 1;
	std::cout << "accum = " << accum(name, name + length) << std::endl; // OK，accum = 1116;


	MyType a[3] = { { 1, 3, 4 },{ 1, 3, 4 },{ 1, 3, 4 } };

	for (int i = 0; i < 3; i++) {
		a[i].print();
		std::cout << std::endl;
	}

	MyType ret = accum2(a, a + 3 );
	ret.print();

	return 0;
}



