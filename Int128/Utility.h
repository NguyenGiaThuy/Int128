#pragma once
#include <algorithm>
#include <bitset>
#include <cctype>
#include <map>
#include <stdexcept>
#include <string>

namespace native {
namespace int128 {
// Provide utility methods to main modules
class Utility {
  friend class QInt;

 public:
  // Convert from ordinary dec string to formatted string
  // Precondition: Input must be any dec strings
  // Postcondition: Output is a formatted dec string
  static std::string formatDecStr(const std::string& decStr) {
    std::string clonedDecStr(DLENGTH, '0');  // Store cloned dec string

    // Convert an ordinary string to a formatted string
    // by correcting its size and format
    if (decStr[0] != '-') {  // Copy decStr and assign leading zeros to
                             // clonedDecStr if decStr indicates positive number
      clonedDecStr.replace(clonedDecStr.size() - decStr.size(), 
          decStr.size(), decStr);
      clonedDecStr = "+" + clonedDecStr;

    } else {                 // Copy decStr with sign and assign leading zeros to clonedDecStr
                             // if decStr indicates negative number
      clonedDecStr.replace(
          clonedDecStr.begin() + (clonedDecStr.size() - decStr.size() + 1),
          clonedDecStr.end(), decStr.begin() + 1, decStr.end());
      clonedDecStr = "-" + clonedDecStr;
    }

    return clonedDecStr;
  }

  // Convert from ordinary hex string to formatted string
  // Precondition: Input must be any hex strings
  // Postcondition: Output is a formatted hex string
  static std::string formatHexStr(const std::string& hexStr) {
    std::string clonedHexStr;  // Store cloned hex string

    // Convert an ordinary string to a formatted string
    // by correcting its size and format (DEPRECATED)
    /*if (clonedHexStr[0] >= 'A' || 
        clonedHexStr[0] >= 'a' ||
        clonedHexStr[0] >= '8') {
      clonedHexStr = std::string(HLENGTH, 'F');
      clonedHexStr.replace(clonedHexStr.size() - hexStr.size(), 
          hexStr.size(), hexStr);
    } else {
      clonedHexStr = std::string(HLENGTH, '0');
      clonedHexStr.replace(clonedHexStr.size() - hexStr.size(),
          hexStr.size(), hexStr);
    }*/

    // Convert an ordinary string to a formatted string
    // by correcting its size and format
    clonedHexStr = std::string(HLENGTH, '0');
    clonedHexStr.replace(clonedHexStr.size() - hexStr.size(), 
        hexStr.size(), hexStr);

    clonedHexStr = toUpper(clonedHexStr);  // Transform string to upper-case
    return clonedHexStr;
  }

 private:
  // Disallow this class to be instantiated
  // Precondition: None
  // Postcondition: None
  Utility() {}

  // Convert from hex or dec macro to maps to allow using
  // contents of macros as searching keys
  // Precondition: Input must be a hex or dec macro 
  // with known size (size must equal macro lengths)
  // Postcondition: Output is a hex or dec map
  static std::map<char, size_t> getMap(const char macros[], size_t size) {
    std::map<char, size_t> map;

    // Create each key-value pair from macro
    for (size_t i = 0; i < size; ++i) {
      map[macros[i]] = i;
    }

    return map;
  }

  // Convert a hex string from lower-case to upper-case
  // Precondition: Input must be any hex strings
  // Postcondition: Output is a upper-case hex string
  static std::string toUpper(const std::string& str) {
    std::string resultStr = str;
    std::transform(resultStr.begin(), resultStr.end(), resultStr.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return resultStr;
  }

  // Sum of two 4-bit bitsets
  // Precondition: Input must be two 4-bit bitsets
  // Postcondition: Output is the sum of 
  // the two former 4-bit bitsets (can be overflow)
  static std::bitset<4> add(const std::bitset<4>& a, const std::bitset<4>& b) {
    std::bitset<4> result;   // Store the result
    std::bitset<1> carrier;  // Store the overflow bit
    size_t i = 0;

    // Evaluate bit by bit of two 4-bit bitsets
    while (i < 4) {
      result[i] = a[i] ^ b[i] ^ carrier[0];  // Evaluate a bit at i position
      carrier = (a[i] & b[i]) |              // Evaluate the overflow bit
          (a[i] & carrier[0]) | 
          (b[i] & carrier[0]);  
      ++i;
    }

    return result;
  }

  // Get 2's complement of a 4-bit bitsets
  // Precondition: Input must be a 4-bit bitset
  // Postcondition: Output is a 2's complement of
  // the former 4-bit bitset (can be overflow)
  static std::bitset<4> getTwoComplement(const std::bitset<4>& bin) {
    return add(std::bitset<4>(1), ~bin);
  }


  // Subtraction of two 4-bit bitsets
  // Precondition: Input must be two 4-bit bitsets
  // Postcondition: Output is the subtraction of
  // the two former 4-bit bitsets (can be overflow)
  static std::bitset<4> subtract(
      const std::bitset<4>& a, const std::bitset<4>& b) {
    std::bitset<4> result;                 // Store the result
    result = add(a, getTwoComplement(b));  // Add a to 2's complement of b
    return result;
  }

 private:
  static constexpr auto HLENGTH = 32;  // Formatted hex string length
  static constexpr auto DLENGTH = 43;  // Formatted dec string length
};

}  // namespace int128
}  // namespace native
