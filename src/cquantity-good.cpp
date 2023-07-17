#include <cmath>
#include <cstddef>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>

// ...
// ****************************************************************************

// Implementation correcte de CQantity

// ...

struct My_type1 {
	std::string m1;
	int m2;
};
constexpr double epsilon = 1e-6;

// Better: compose using memberwise application of the class's members operations.
// This typically yields the finest (most discriminative) relational operators.
struct CQuantity {
	// Demontrable que ceci n'est pas transitif.
	bool operator==(CQuantity const &rhs) {
		return std::abs(this->value - rhs.value) < epsilon;
	}

	// Definition deduite de ==
	// bool operator!=(CQuantity const& rhs)
	// {
	// 	return !(*this == rhs);
	// }
	bool operator<=(CQuantity const &rhs) {

		return this->value < rhs.value || *this == rhs;

		// question: peut on faire commuter les comparaisons?
		// return *this == rhs || this->value < rhs.value;
	}

	// operation deduite de < et ==
	bool operator<(CQuantity const &rhs) {
		return *this <= rhs && !(*this == rhs);

		// question: peut on faire commuter les comparaisons?
		// return *this <= rhs && *this != rhs;
	}

	double value;
};

template <> struct std::hash<CQuantity> {
	std::size_t operator()(CQuantity const &val) {
		return std::hash<double>{}(val.value);
	}
};


// Furthermore, we may define special purpose 'coarser

using namespace std;
int main() {

	unordered_map<CQuantity, int> v1;

	map<CQuantity, int> m1;

	My_type1 v3 = {"abc", 4};
	map<My_type1, int> v4;
	// v4.insert(, InputIterator last)

	cout << "hello world\n";
}

// Pourquoi on est porte a comparer avec abs(x - y) < epsilon?
// - critere de convergence
// - unit test
// - ...
// Mais abs(x-y) < e n'est pas une relation d'equivalence,
//

// ...
// ****************************************************************************

// Implementation correcte de CQantity

// ...

// ****************************************************************************
// Scratchpad
// auto operator<=>(T lhs, Trs) = default;
// auto operator==(T lhs, Trs) = default;

// a <=> b -> returns (a <=> b) < 0 ou (a <=> b) > 0 ou (a <=> b) == 0

// {
// 	constexpr T var;
// 	operator=()
// 	friend operator+()
// 	friend operator==(T a, T b)
// 	{
// 		static T var;

// 	}
// }
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
