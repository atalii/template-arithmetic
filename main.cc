#include <concepts>

class Zero {};

template <typename T>
class S {};

template <typename LHS, typename RHS>
class Add {};

template <typename RHS>
class Add<Zero, RHS> {
public:
	using Result = RHS;
};

template <typename LHS, typename RHS>
class Add<S<LHS>, RHS> {
public:
	using Result = S<typename Add<LHS, RHS>::Result>;
};

template <typename LHS, typename RHS>
class LEThan {};

template <typename RHS>
class LEThan<Zero, RHS> 
{
public:
	using Result = std::true_type;
};

template <typename LHS>
class LEThan<S<LHS>, Zero> 
{
public:
	using Result = std::false_type;
};

template <typename LHS, typename RHS>
class LEThan<S<LHS>, S<RHS>>
{
public:
	using Result = LEThan<LHS, RHS>::Result;
};

template <typename LHS, typename RHS>
class GThan {
public:
	using Result = typename std::conditional<
		LEThan<LHS, RHS>::Result::value,
		std::false_type,
		std::true_type
	>::type;
};

using One = S<Zero>;
using Two = S<One>;
using Three = S<Two>;
using Four = S<Three>;

using OnePlusZero = Add<One, Zero>::Result;
static_assert(std::is_same<OnePlusZero, One>::value, "1 + 0 = 1.");

using TwoPlusTwo = Add<Two, Two>::Result;
static_assert(std::is_same<TwoPlusTwo, Four>::value, "2 + 2 = 4.");

using ZeroLEOne = LEThan<Zero, One>::Result;
static_assert(std::is_same<ZeroLEOne, std::true_type>::value, "0 <= 1");

using OneLEZero = LEThan<One, Zero>::Result;
static_assert(std::is_same<OneLEZero, std::false_type>::value, "1 </= 0");

using TwoLEFour = LEThan<Two, Four>::Result;
static_assert(std::is_same<TwoLEFour, std::true_type>::value, "2 <= 4");

using OneGZero = GThan<One, Zero>::Result;
static_assert(std::is_same<OneGZero, std::true_type>::value, "1 > 0");
