#include <cmath>
#include <cstddef>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>

struct CQuantity {

	double value;
	friend auto operator<=>(CQuantity const&, CQuantity const&) = default;
};

template<> struct std::hash<CQuantity> {
	std::size_t operator()(CQuantity const& val) { return std::hash<double>{}(val.value); }
};

// Furthermore, we may define special purpose 'coarser

using namespace std;
int main() { cout << "all done!\n"; }
