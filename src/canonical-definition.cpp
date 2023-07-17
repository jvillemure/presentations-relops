#include <cmath>
#include <cstddef>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>

// The usual canonical implemtation of the equality and order relations does a memberwise
// lexicographic comparison. This intuitively yields the most fine grained equality and order
// relationship on a given type, and is normally the operation we want to use in most context. Other
// less discriminative (coarse grained) equality and orders can be defined for other specific
// contexts; this is normally done by passing a function object to the algorithm or data structure
// that expose a customization point for these relationships (if there is no such cust point, then
// we can only use the classe's canonical operators). A non canonical implementation should also be
// consistent with the canonical operators.

// Golden rules for the equality:

// 1) Equality preservation. As defined in the standard.
//    Informally, equality preservation means that for two objects o1 and o2 that compare equal (o1
//    == o2), for any expression (expr(o1)) that uses the value of o1, we could substitute the
//    object o1 by the object o2 in the expression, and the evaluation of (expr(o2)) would have
//    identical semant

namespace ns1 {

struct My_type1 {

	std::string m1;
	int m2;

	// For symmetric treatments of arguments, binary operator overloads should be
	// non-member function in the same namespace
	// Ideally, make the function noexcept, this should be the case if all data members operator==
	// are.
	friend bool operator==(My_type1 const& lhs, My_type1 const& rhs) noexcept
	{
		// Lexicographic memberwise equality
		return lhs.m1 == rhs.m1 && lhs.m2 == rhs.m2;
	}

	friend bool operator<(My_type1 const& lhs, My_type1 const& rhs) noexcept
	{
		// Lexicographic memberwise equality
		return lhs.m1 < rhs.m1 && lhs.m2 < rhs.m2;
	}
};

} // namespace ns1

// An example of a coarser comparison

using namespace std;
using namespace ns1;
int main()
{

	My_type1 v3 = {"abc", 4};
	map<My_type1, int> v4;

	cout << "canonical definitions\n";
}
