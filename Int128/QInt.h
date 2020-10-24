#pragma once

#include "Utilities.h"

#define BIN_LENGTH 128
#define HEX_LENGTH 32
#define DEC_LENGTH 43
#define HEX_MAP "0123456789ABCDEF"
#define DEC_MAP "0123456789"
#define INT128_MAX "170141183460469231731687303715884105727"
#define INT128_MIN "-170141183460469231731687303715884105728"

namespace Native {
namespace Int128 {
class QInt {
 public:
  QInt();
  QInt(const std::string&, int);
  QInt& operator=(const QInt&);

  std::bitset<BIN_LENGTH>::reference operator[](size_t);
  constexpr bool operator[](size_t) const;

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
  QInt rol(size_t) const;
  QInt ror(size_t) const;

  std::string getContent() const;

 public:
  static std::string hexToBin(const std::string&);
  static std::string binToHex(const std::string&);

  static std::string decToBin(const std::string&);
  static std::string binToDec(const std::string&);

 private:
  QInt _getTwoComplement() const;

 private:
  std::bitset<BIN_LENGTH> _bin;
};
}  // namespace Int128
}  // namespace Native
