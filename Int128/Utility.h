#pragma once
#include <algorithm>
#include <bitset>
#include <cctype>
#include <map>
#include <stdexcept>
#include <string>

namespace native {
namespace int128 {
class Utility {
  friend class QInt;

 public:
  static std::string validateDec(const std::string& decStr) {
    std::string clonedDecStr(DLENGTH, '0');
    if (decStr[0] != '-') {         // Copy "decStr" and assign leading zeros
      clonedDecStr.replace(
          clonedDecStr.size() - decStr.size(), 
          decStr.size(), decStr);
      clonedDecStr = "+" + clonedDecStr;

    } else {                        // Copy "decStr" with sign and assign leading zeros
      clonedDecStr.replace(
          clonedDecStr.begin() + 
          (clonedDecStr.size() - decStr.size() + 1),
          clonedDecStr.end(), 
          decStr.begin() + 1, decStr.end());
      clonedDecStr = "-" + clonedDecStr;
    }
    
    return clonedDecStr;
  }

  static std::string validateHex(const std::string& hexStr) {
    std::string clonedHexStr;      // Store cloned hexadecmial string

    if (clonedHexStr[0] >= 'A' ||  // Assign leading 'F's
        clonedHexStr[0] >= 'a' || 
        clonedHexStr[0] >= '8') {
      clonedHexStr = std::string(HLENGTH, 'F');

    } else {                       // Assign leading zeros
      clonedHexStr = std::string(HLENGTH, '0');
    }

    clonedHexStr.replace(  // Copy "hexStr"
        clonedHexStr.size() - hexStr.size(), 
        hexStr.size(), hexStr);
    clonedHexStr =         // Transform all characters to upper case
        toUpper(clonedHexStr);

    return clonedHexStr;
  }

 private:
  Utility() {}

  static std::map<char, size_t> getMap(const char macros[], size_t size) {
    std::map<char, size_t> map;
    for (size_t i = 0; i < size; ++i) {
      map[macros[i]] = i;
    }

    return map;
  }

  static std::string toUpper(const std::string& str) {
    std::string resultStr = str;
    std::transform(resultStr.begin(), resultStr.end(), resultStr.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return resultStr;
  }

  static std::bitset<4> add(const std::bitset<4>& a, const std::bitset<4>& b) {
    std::bitset<4> result;
    std::bitset<1> carrier;

    size_t i = 0;
    while (i < 4) {
      result[i] = a[i] ^ b[i] ^ carrier[0];
      carrier = (a[i] & b[i]) | (a[i] & carrier[0]) | (b[i] & carrier[0]);
      ++i;
    }

    return result;
  }

  static std::bitset<4> getTwoComplement(const std::bitset<4>& bin) {
    return add(std::bitset<4>(1), ~bin);
  }

  static std::bitset<4> subtract(const std::bitset<4>& a,
                                 const std::bitset<4>& b) {
    std::bitset<4> result;
    result = add(a, getTwoComplement(b));
    return result;
  }

 private:
  static constexpr auto HLENGTH = 32;
  static constexpr auto DLENGTH = 43;
};

}  // namespace int128
}  // namespace native
