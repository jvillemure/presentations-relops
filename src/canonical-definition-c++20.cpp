#include <cassert>
#include <compare>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

// Implementing equality and ordering in C++20

// In c++20, we only need to define the 3way comparison operator<=> to define all ordering relationships,
// and possibly the equality also.
// All uses of the operators (<, >, <=, >=, ==, !=) for the argument types will be automatically be rewritten as
// a comparison with the result of the 3 way comparison.

// Heterogenous arguments are also possible, and are 'automatically' generated
// in a sensible way.

// The 3 way comparison yields one of 4 possible values, which can be compared against
// a literal '0' using any one of (<, >, <=, >=, ==)

// (a <=> b) < 0 if a < b,
// (a <=> b) > 0 if a > b,
// (a <=> b) == 0 if a and b are equal/equivalent.

// The result type of the 3 way comparison depends on the classification of the composite
// operations induced by the classe's members:
// - std::strong_ordering if all data members' operator<=> yields std::strong_ordering
// - std::weak_ordering if some data member's operator<=> yields std::weak_ordering
//   and all others yields std::strong_ordering
// - std::partial_ordering otherwise

// Non floating point built-in types' ordering categories are std::strong_ordering

// Like copy/move assign/ctor and dtor, in c++20 any comparison operators can be defaulted.

namespace ns1 {

struct My_type1 {

	std::string m1;
	int m2;

	friend auto operator<=>(My_type1 const&, My_type1 const&) = default;
};
}

template<>
struct std::hash<ns1::My_type1>
{
	std::size_t operator()(ns1::My_type1 const& o) const noexcept
	{
		std::size_t h1 = std::hash<std::string>{}(o.m1);
		std::size_t h2 = std::hash<int>{}(o.m2);
		return h1 ^ (h2 << 1); // or use boost::hash_combine
	}

};

using namespace std;
using namespace ns1;
int main()
{

	My_type1 v1 = {"abc"s, 4};
	My_type1 v2 = {"abc"s, 0};
	My_type1 v3 = {"def"s, 0};
	My_type1 v4 = {"hiijkl"s, -10};
	auto v11 = v1;
	My_type1 v12;
	v12 = v1;
	My_type1 v13 = {"abc"s, 4};

	// Verify the fundamental properties of the ordering

	// Antireflexive
	assert(!(v1 < v1));
	assert(!(v2 < v2));
	assert(!(v3 < v3));
	assert(!(v4 < v4));

	// Asymmetry
	assert(v1 < v3);
	assert(!(v3 < v1));

	assert(v2 < v1);
	assert(!(v1 < v2));

	assert(v2 < v1);
	assert(!(v1 < v2));

	assert(v2 < v3);
	assert(!(v3 < v2));

	assert(v3 < v4);
	assert(!(v4 < v3));

	// Transitivity
	assert(v2 < v3 && v3 < v4);
	assert(v2 < v4);

	assert(v1 < v3 && v3 < v4);
	assert(v1 < v4);

	assert(v2 < v1 && v1 < v4);
	assert(v2 < v4);

	// Verify the fundamental properties of the equality

	// Reflexivity
	assert(v1 == v1);
	assert(v11 == v11);
	assert(v12 == v12);
	assert(v13 == v13);

	// Symmetry
	assert(v1 == v11);
	assert(v11 == v1);

	assert(v11 == v12);
	assert(v12 == v11);

	assert(v11 == v13);
	assert(v13 == v11);

	// Transitivity
	assert(v1 == v11 && v11 == v12);
	assert(v1 == v12);

	assert(v1 == v13 && v13 == v11);
	assert(v1 == v11);

	// Check that equality distinguishes objects with distinct values
	assert(!(v1 == v2));
	assert(!(v2 == v1));

	assert(!(v1 == v3));
	assert(!(v3 == v1));

	assert(!(v1 == v4));
	assert(!(v4 == v1));

	// Consistency with hash calculations:
	hash<My_type1> myhash{};
	assert(myhash(v1) != myhash(v3));
	assert(myhash(v2) != myhash(v3));
	assert(myhash(v1) != myhash(v4));

	assert(myhash(v1) == myhash(v11));
	assert(myhash(v12) == myhash(v11));

	cout << "All OK! done\n";
}
