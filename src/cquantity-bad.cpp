#include <cassert>
#include <cmath>
#include <cstddef>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

// Examples of non semantically conformant implementations of Equality and the implications.

// I often came across non conforming implementations of Equality.
// This leads to problems and erratic behavior of data structures and algorithms
// that expect to be given and equality operation.

constexpr double epsilon = 1e-6;

// Bad: this class's operator == and < are not an equivalence relation and an strict weak order
// relation, respectively.
struct CQuantity {
	// BAD: this relationship is not an Equality at all!
	// For one thing, it violates Transitivity.
	// Secondly, it doesn't depend solely on the argument's value.
	// This is because the relation it implements has 3 operand.
	bool operator==(CQuantity const& rhs) { return std::abs(this->value - rhs.value) < epsilon; }

	// Definition deduite de ==
	// bool operator!=(CQuantity const& rhs)
	// {
	// 	return !(*this == rhs);
	// }
	bool operator<=(CQuantity const& rhs)
	{

		return this->value < rhs.value || *this == rhs;

		// question: peut on faire commuter les comparaisons?
		// return *this == rhs || this->value < rhs.value;
	}

	// BAD: this is not a Strict Weak Ordering, it cannot be used for sorting or ordered data
	// structures
	bool operator<(CQuantity const& rhs)
	{
		return *this <= rhs && !(*this == rhs);

		// question: peut on faire commuter les comparaisons?
		// return *this <= rhs && *this != rhs;
	}

	double value;
};

template<> struct std::hash<CQuantity> {
	// How can we implement a hashing function with a behavior consistent
	// with the equality operator defined above in CQuantity?????????
	// ...
	// It simply is impossible (mathematically)!
	// For demonstation purpose, let's keep the canonical default:
	// calculate the hash of the wrapped value.
	std::size_t operator()(CQuantity const& val) { return std::hash<double>{}(val.value); }
};

// It is fundamentally important to differnciate the notion of Equality
// from the notion of Closeness. Closeness is not transitive, so it
// cannot be used anywhere an Equality is required. Furthermore, a definition of Ordering
// derived from Closeness only leads to a Partial Ordering, thus it is not good
// for algorithms and data structures. Closeness is not a 2 argument relationship:
// two values cannot be sensibly compared without a reference scale, and there is no
// universal scale that could be used for all uses of the Closeness relationship.
// For instance, on an atomic scale 1 micron (1e-6) is not close to 1.01 micron (1.01e-6),
// on the other hand, on an astronomic scale 1micron is close to 1.01micron.

using namespace std;
int main()
{

	// CQuantity v1 = {"abc"s, 4};
	// CQuantity v2 = {"abc"s, 0};
	// CQuantity v3 = {"def"s, 0};
	// CQuantity v4 = {"hiijkl"s, -10};
	// auto v11 = v1;
	// CQuantity v12;
	// v12 = v1;
	// CQuantity v13 = {"abc"s, 4};

	CQuantity v1 = {1.};
	CQuantity v2 = {.9999999};
	CQuantity v3 = {.99999995};
	CQuantity v4 = {.99999996};
	auto v11 = v1;
	CQuantity v12;
	v12 = v1;
	CQuantity v13 = {.999999951};

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
	hash<CQuantity> myhash{};
	assert(myhash(v1) != myhash(v3));
	assert(myhash(v2) != myhash(v3));
	assert(myhash(v1) != myhash(v4));

	assert(myhash(v1) == myhash(v11));
	assert(myhash(v12) == myhash(v11));

	cout << "All OK! done\n";
}
// Pourquoi on est porte a comparer avec abs(x - y) < epsilon?
// - critere de convergence
// - unit test
// - ...
// Mais abs(x-y) < e n'est pas une relation d'equivalence,
//

// a+ b == b+a
// "abc"s +"def"s
// "def"s + "abc"s

// Gr Th:
// ("abc"s +"def"s) +"g"s
// "abc"s + ("def"s +"g"s)

// x + nullobj = x

// a == b;
// b == a;

// constexpr double e = 1e-6;

// class CQuantity
// {
// 	// Demontrable que ceci n'est pas transitif
// 	bool operator==(CQuantity const & rhs)
// 	{&
// 		return std::abs(this->value - rhs.value) < e;
// 	}
// 	bool operator<= (CQuantity const& rhs)
// 	{

// 		return this->value < rhs.value && *this == rhs;
// 	}
// }

// e = .7;
//    .1,     .1      ,.1    ,.1
// x1 == x2, x2 == x3, ...., x18 == x19; x1 != x19

// x1 == x19;

// std::map<CQuantity, int> // le map ne
// <

// std::unordered_map<CQuantity, int>
// ==
// std::hash<CQuantity>

// !x < y && ! y< x //

// NAN == NAN // toujours faux
// NAN< NAN // toujours faux
