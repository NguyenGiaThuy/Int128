#pragma once

#include "QInt.h"
#include "utility.h"

namespace native {
namespace float128 {
class QFloat {
 public:
  // Convert from decimal number to binary number
  // Precondition: Input must be a formatted string
  // Postcondition: Output is a floating point binary string
  // *NOTE*: Number has more than 34 digits
  // may get precision loss
  static std::string decToBin(const std::string&);

  // Convert from binary number to decimal number
  // Precondition: Input must be a formatted string
  // Postcondition: Output is a floating point decimal string
  // *NOTE*: Not working when the number has more than 34 digits
  // and the number is in denormalized form
  static std::string binToDec(const std::string&);

 private:
  static std::string getExponentStr(const std::string&);
  static std::string getIntegralStr(const std::string&);
  static std::string getFractionalStr(const std::string&);

  static std::string exponentDecToBin(const std::string&);
  static std::string integralDecToBin(const std::string&);
  static std::string fractionalDecToBin(const std::string&);

  // Format the binary string to a certain form to help
  // process easily
  // Precondition: Input must be a formatted string
  // Postcondition: Output is a special formatted string
  static std::string formatBinStr(const std::string&);
};
}  // namespace float128
}  // namespace native
