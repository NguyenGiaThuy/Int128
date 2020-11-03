#pragma once

#include "utility.h"

namespace native {
namespace int128 {
class QInt {
 public:
  QInt();

  // Instantate a QInt from any input strings (bin, dec, hex)
  // Precondition: Input strings must be compatible with
  // base (bin - 2, dec - 10, hex - 16)
  // Postcondition: A QInt instantiated
  QInt(const std::string&, int);

  QInt& operator=(const QInt&);
  QInt operator+(const QInt&) const;
  QInt operator-(const QInt&) const;
  QInt operator*(const QInt&) const;
  QInt operator/(const QInt&) const;
  QInt operator%(const QInt&) const;
  QInt exp(size_t) const;

  bool operator<(const QInt&) const;
  bool operator>(const QInt&) const;
  bool operator<=(const QInt&) const;
  bool operator>=(const QInt&) const;
  bool operator==(const QInt&) const;
  bool operator!=(const QInt&) const;

  QInt operator<<(size_t) const;
  QInt operator>>(size_t) const;
  QInt operator&(const QInt&) const;
  QInt operator|(const QInt&) const;
  QInt operator^(const QInt&) const;
  QInt operator~() const;

  // Rotate a QInt to the left
  // Precondition: Input must be an unsigned integer
  // Postcondition: Output is a rotated-left QInt
  QInt rol(size_t) const;

  // Rotate a QInt to the right
  // Precondition: Input must be an unsigned integer
  // Postcondition: Output is a rotated-right QInt
  QInt ror(size_t) const;

  // Get the content as a formatted bin string
  // Precondition: None
  // Postcondition: Output is a formatted bin string
  std::string getBin() const;

  // Get the content as a normalized dec string
  // Precondition: None
  // Postcondition: Output is a normalized dec string
  std::string getDec() const;

 public:
  // Precondition: All inputs must be formatted strings
  // Postcondition: All outputs are formatted strings

  // Convert from hexadecimal number to binary number
  static std::string hexToBin(const std::string&);

  // Convert from binary number to hexadecimal number
  static std::string binToHex(const std::string&);

  // Convert from decimal number to binary number
  static std::string decToBin(const std::string&);

  // Convert from binary number to decimal number
  static std::string binToDec(const std::string&);

 private:
  QInt getTwoComplement() const;

  // Enable subsription operator (editable)
  // Precondition: Input must be a unsigned integer (-1 < i < BIN_LENGTH)
  // Postcondition: Output is a bit at position i (editable)
  std::bitset<INT128_BIN_LENGTH>::reference operator[](size_t);

  // Enable subsription operator (read-only)
  // Precondition: Input must be a unsigned integer (-1 < i < BIN_LENGTH)
  // Postcondition: Output is a bit at position i (read-only)
  constexpr bool operator[](size_t) const;

 private:
  std::bitset<INT128_BIN_LENGTH> _bin;  // Store a 128-bit binary number
};
}  // namespace int128
}  // namespace native
