#pragma once
#include <algorithm>
#include <bitset>
#include <cctype>
#include <map>
#include <stdexcept>
#include <string>
#include <cmath>

#include "int128_limits.h"
#include "float128_limits.h"

namespace native {
// Provide utility methods to main modules
namespace utility {
#define HEX_MAP "0123456789ABCDEF"
#define DEC_MAP "0123456789"

// Convert from ordinary dec string to formatted string
// Precondition: Input must be any dec strings compatible
// with type ('i' - int128, 'f' - float128)
// Postcondition: Output is a formatted dec string
std::string formatDecStr(const std::string&, const char);

// Convert from ordinary hex string to formatted string
// Precondition: Input must be any hex strings
// Postcondition: Output is a formatted hex string
std::string formatHexStr(const std::string&);

// Convert from hex or dec macro to maps to allow using
// contents of macros as searching keys
// Precondition: Input must be a hex or dec macro
// with known size (size must equal macro lengths)
// Postcondition: Output is a hex or dec map
std::map<char, size_t> getMap(const char[], size_t);

// Convert a hex string from lower-case to upper-case
// Precondition: Input must be any hex strings
// Postcondition: Output is a upper-case hex string
std::string toUpper(const std::string&);

// Sum of two 4-bit bitsets
// Precondition: Input must be two 4-bit bitsets
// Postcondition: Output is the sum of
// the two former 4-bit bitsets (can be overflow)
std::bitset<4> add(const std::bitset<4>&, const std::bitset<4>&);

// Get 2's complement of a 4-bit bitsets
// Precondition: Input must be a 4-bit bitset
// Postcondition: Output is a 2's complement of
// the former 4-bit bitset (can be overflow)
std::bitset<4> getTwoComplement(const std::bitset<4>&);

// Subtraction of two 4-bit bitsets
// Precondition: Input must be two 4-bit bitsets
// Postcondition: Output is the subtraction of
// the two former 4-bit bitsets (can be overflow)
std::bitset<4> subtract(const std::bitset<4>& a, const std::bitset<4>&);
}  // namespace utility
}  // namespace native
