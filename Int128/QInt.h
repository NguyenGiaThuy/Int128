#pragma once

#include "Utility.h"

#define BIN_LENGTH 128
#define HEX_LENGTH 32
#define DEC_LENGTH 43
#define HEX_MAP "0123456789ABCDEF"
#define DEC_MAP "0123456789"
#define INT128_MAX "170141183460469231731687303715884105727"
#define INT128_MIN "-170141183460469231731687303715884105728"

namespace native {
namespace int128 {
class QInt {
 public:
  QInt();

  // Instantate a QInt from any input strings (bin, dec, hex)
  // Precondition: Input strings must be compatible with
  // type numbers (bin - 2, dec - 10, hex - 16)
  // Postcondition: A QInt instantiated
  QInt(const std::string&, int);

  QInt& operator=(const QInt&);
  QInt operator+(const QInt&) const;
  QInt operator-(const QInt&) const;
  QInt operator*(const QInt&) const;
  QInt operator/(const QInt&) const;
  QInt operator%(const QInt&) const;

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
  std::bitset<BIN_LENGTH>::reference operator[](size_t);

  // Enable subsription operator (read-only)
  // Precondition: Input must be a unsigned integer (-1 < i < BIN_LENGTH)
  // Postcondition: Output is a bit at position i (read-only)
  constexpr bool operator[](size_t) const;

 private:
  std::bitset<BIN_LENGTH> _bin;  // Store a 128-bit binary number
};
}  // namespace int128
}  // namespace native
