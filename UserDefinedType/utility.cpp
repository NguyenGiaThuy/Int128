#include "utility.h"

namespace native {
namespace utility {
std::string formatDecStr(const std::string& decStr, const char type) {
  std::string resultStr;

  switch (type) {
    case 'i':
      resultStr = std::string(INT128_DEC_LENGTH, '0');

      // Convert an ordinary string to a formatted string
      // by correcting its size and format
      if (decStr[0] != '-') {                   // Copy decStr and assign leading zeros to
                                                // resultStr if decStr indicates positive number
        resultStr.replace(resultStr.size() - decStr.size(), decStr.size(),
                             decStr);
        resultStr = "+" + resultStr;

      } else {                                  // Copy decStr with sign and assign leading zeros to
                                                // clonedDecStr if decStr indicates negative number
        resultStr.replace(
            resultStr.begin() + (resultStr.size() - decStr.size() + 1),
            resultStr.end(), decStr.begin() + 1, decStr.end());
        resultStr = "-" + resultStr;
      }

      break;
    case 'f':
      // Case the number is zero
      if (decStr.find_first_not_of('0') == std::string::npos) {
        resultStr = "0.0e0";
        return resultStr;
      }

      // Strip the leading zeros and the sign and assign a dot
      //  and a zero to form a floating point number
      resultStr =
          (decStr[0] == '-') ? decStr.substr(decStr.find_first_not_of('0', 1))
                             : decStr.substr(decStr.find_first_not_of('0'));
      resultStr += (resultStr.find('.') == std::string::npos) ? ".0" : "";

      int exp = 0;
      double roundedDb;

      // Handle when the number is too long
      if (resultStr.length() > FLOAT128_PRECISION + 1) {
        switch (resultStr[0]) {
          case '.':  // Case the number is in the form: 0.f
            // Strip the overflow number and round the last number
            exp = 0 - resultStr.find_first_not_of('0', 1);
            resultStr = resultStr.erase(resultStr.find('.'), 1);
            try {  // Round up to 5 last digits and assign dot if 
                   // the fraction does not contains only zeros
              roundedDb =
                  std::stod(resultStr.substr(abs(exp), 5).insert(1, 1, '.'));
              roundedDb = round(roundedDb);
            } catch (std::out_of_range) {  // Round the last digit and assign dot
                                           // in case the fraction contains only zeros
                                           // and the last digit is non-zero
              roundedDb =
                  std::stod(resultStr.substr(abs(exp) - 1).insert(1, 1, '.'));
              roundedDb = round(roundedDb);
            }

            // Asign the rounding part to the fraction
            // and erase the overflow numbers
            resultStr = resultStr.erase(abs(exp));
            resultStr = resultStr.replace(
                abs(exp), 1, "." + std::to_string(roundedDb).erase(1, 1));
            break;
          default:  // Case the number is in the form: [1-9].f
            // Has the same logic as case 0.f
            exp = resultStr.find('.') - FLOAT128_PRECISION;
            resultStr = resultStr.erase(resultStr.find('.'), 1);
            roundedDb = std::stod(
                resultStr.substr(FLOAT128_PRECISION - 1, 5).insert(1, 1, '.'));
            roundedDb = round(roundedDb);

            resultStr = resultStr.erase(FLOAT128_PRECISION);
            resultStr = resultStr.replace(FLOAT128_PRECISION - 1, 1,
                                          std::to_string(roundedDb));
            break;
        }
      }

      // Build the formatted dec string by assign the a zero
      // before the dot and assign the sign back, insert the 'e' 
      // to indicate exponent
      resultStr = resultStr.substr(resultStr.find_first_not_of('0'));
      resultStr = (resultStr[0] == '.') ? "0" + resultStr : resultStr;
      resultStr = (decStr[0] == '-') ? "-" + resultStr : resultStr;
      resultStr += "e" + std::to_string(exp);

      break;
  }

  return resultStr;
}

std::string formatHexStr(const std::string& hexStr) {
  std::string resultStr;

  // Convert an ordinary string to a formatted string
  // by correcting its size and format
  if (resultStr[0] >= 'A' || resultStr[0] >= 'a' ||
      resultStr[0] >= '8') {
    resultStr = std::string(INT128_HEX_LENGTH, 'F');
    resultStr.replace(resultStr.size() - hexStr.size(), 
        hexStr.size(), hexStr);
  } else {
    resultStr = std::string(INT128_HEX_LENGTH, '0');
    resultStr.replace(resultStr.size() - hexStr.size(), 
        hexStr.size(), hexStr);
  }

  resultStr = toUpper(resultStr);  // Transform string to upper-case
  return resultStr;
}

 

std::map<char, size_t> getMap(const char macros[], size_t size) {
  std::map<char, size_t> map;

  // Create each key-value pair from macro
  for (size_t i = 0; i < size; ++i) {
    map[macros[i]] = i;
  }

  return map;
}

std::string toUpper(const std::string& str) {
  std::string resultStr = str;
  std::transform(resultStr.begin(), resultStr.end(), resultStr.begin(),
                 [](unsigned char c) { return std::toupper(c); });
  return resultStr;
}

std::bitset<4> add(const std::bitset<4>& a, const std::bitset<4>& b) {
  std::bitset<4> result;   // Store the result
  std::bitset<1> carrier;  // Store the overflow bit
  size_t i = 0;

  // Evaluate bit by bit of two 4-bit bitsets
  while (i < 4) {
    result[i] = a[i] ^ b[i] ^ carrier[0];  // Evaluate a bit at i position
    carrier = (a[i] & b[i]) |              // Evaluate the overflow bit
              (a[i] & carrier[0]) | (b[i] & carrier[0]);
    ++i;
  }

  return result;
}

std::bitset<4> getTwoComplement(const std::bitset<4>& bin) {
  return add(std::bitset<4>(1), ~bin);
}

std::bitset<4> subtract(const std::bitset<4>& a,
                               const std::bitset<4>& b) {
  std::bitset<4> result;                 // Store the result
  result = add(a, getTwoComplement(b));  // Add a to 2's complement of b
  return result;
}
}  // namespace utility
}  // namespace native