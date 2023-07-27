#include <cassert>
#include <cmath>
#include <cstddef>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>

// Implementing Equality and ordering

// The usual canonical implemtation of the equality and order relations does a memberwise
// lexicographic comparison. This intuitively yields the most fine grained equality and order
// relationship on a given type, and is normally the operation we want to use in most context. Other
// less discriminative (coarse grained) equality and orders can be defined for other specific
// contexts; this is normally done by passing a function object to the algorithm or data structure
// that expose a customization point for these relationships (if there is no such cust point, then
// we can only use the classe's canonical operators). A non canonical implementation should also be
// consistent with the canonical operators.

namespace ns1 {

struct My_type1 {

	std::string m1;
	int m2;

	// For symmetric treatments of arguments, binary operator overloads should be
	// non-member function in the same namespace as the type of the operand.
	// A common way to do this in C++ is to declare the operator
	// as friend function and define it directly within the class; this
	// makes the function only visible through ADL.

	// Ideally, make the function noexcept, this should be the case if all data members operator==
	// are. (Also consider make it constexpr if the type is Literal).
	friend bool operator==(My_type1 const& lhs, My_type1 const& rhs) noexcept
	{
		// Lexicographic memberwise equality
		return lhs.m1 == rhs.m1 && lhs.m2 == rhs.m2;
	}

	// When both Equality and Ordering are defined for a class,
	// you should take some time and ensure that the semantics of operator== and operator<
	// are consistent.

	// 1) Requirement: if 'x1 == x2' then '!(x1 < x2) && !(x2 < x1)'
	//    in other words, equal elements do not compare smaller or larger
	// 2) Recommendation: '!(x1 < x2) && !(x2 < x1)' then 'x1 == x2'
	//    In other words, incomparable elements should be equal.

	friend bool operator<(My_type1 const& lhs, My_type1 const& rhs) noexcept
	{
		// Lexicographic memberwise ordering
		// Note, implementation need only use the operator< of its members.
		// Also, note that the operation== may be more specific than
		// the operation (a < b) && !(b < a), but usually both should be equivalent operations.
		return lhs.m1 < rhs.m1 || !(rhs.m1 < lhs.m1) && lhs.m2 < rhs.m2;

		// Alternative implementation. If this would not be equivalent to the above,
		// then review your design as this could be a sign of a flaw...
		// return lhs.m1 < rhs.m1 || !(rhs.m1 < lhs.m1) && lhs.m2 < rhs.m2;
	}

	// Every other operations should be defined in terms of the two above

	friend bool operator!=(My_type1 const& lhs, My_type1 const& rhs) noexcept
	{
		return !(lhs == rhs);
	}

	friend bool operator>(My_type1 const& lhs, My_type1 const& rhs) noexcept
	{ //
		return rhs < lhs;
	}

	// The two following may be implemented in two ways, and if the < and == have
	// been implemented well and in a canonical way, this should be equivalent.
	// If their behavior would not be equivalent, then, it is important to
	// undertand the implications on which one to use; and this may be a sign of a design flaw.

	// My personal pref is to not use == in implementing those.
	// the other alternative would be to use ==.
	friend bool operator<=(My_type1 const& lhs, My_type1 const& rhs) noexcept
	{
		return !(lhs > rhs);
	}
	friend bool operator>=(My_type1 const& lhs, My_type1 const& rhs) noexcept
	{ //
		return rhs <= lhs;
	}
};

} // namespace ns1
template<> struct std::hash<ns1::My_type1> {
	std::size_t operator()(ns1::My_type1 const& o) const noexcept
	{
		std::size_t h1 = std::hash<std::string>{}(o.m1);
		std::size_t h2 = std::hash<int>{}(o.m2);
		return h1 ^ (h2 << 1); // or use boost::hash_combine
	}
};

// An example of a coarser comparison
// ....

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
