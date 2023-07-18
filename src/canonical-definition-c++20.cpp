#include <cmath>
#include <cstddef>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <compare>

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

// The result type of the 3 way comparison depends on the 'semantic level' of the composite
// operations induced by the classe's members. There are

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
int main() {

	My_type1 v3 = {"abc", 4};
	map<My_type1, int> v4;
	unordered_map<My_type1, int> v5 ;

	cout << "canonical definitions\n";
}
