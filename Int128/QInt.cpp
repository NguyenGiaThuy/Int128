#include "QInt.h"

namespace native {
namespace int128 {
QInt::QInt() { (*this)._bin = std::bitset<BIN_LENGTH>(); }

QInt::QInt(const std::string& str, int format) {
  switch (format) {
    case 2:
      (*this)._bin = std::bitset<BIN_LENGTH>(str);
      break;
    case 10:
      (*this)._bin =
          std::bitset<BIN_LENGTH>(QInt::decToBin(Utility::validateDec(str)));
      break;
    case 16:
      (*this)._bin =
          std::bitset<BIN_LENGTH>(QInt::hexToBin(Utility::validateHex(str)));
      break;
  }
}

QInt& QInt::operator=(const QInt& other) {
  (*this)._bin = other._bin;
  return *this;
}

std::bitset<BIN_LENGTH>::reference QInt::operator[](size_t pos) {
  return (*this)._bin[pos];
}

constexpr bool QInt::operator[](size_t pos) const {
  return (*this)._bin[pos];
}

QInt QInt::operator+(const QInt& other) const {
  QInt result;
  std::bitset<1> carrier;

  size_t i = 0;
  while (i < BIN_LENGTH) {
    result[i] = (*this)[i] ^ other[i] ^ carrier[0];
    carrier = ((*this)[i] & other[i]) | ((*this)[i] & carrier[0]) |
              (other[i] & carrier[0]);
    ++i;
  }

  return result;
}

QInt QInt::operator-(const QInt& other) const {
  QInt result;
  result = *this + other._getTwoComplement();
  return result;
}

QInt QInt::operator*(const QInt& other) const {
  QInt multiplier =
      ((*this)[BIN_LENGTH - 1] ^ 1) ? *this : (*this)._getTwoComplement();
  QInt multiplicand =
      (other[BIN_LENGTH - 1] ^ 1) ? other : other._getTwoComplement();
  QInt result;

  size_t i = 0;
  while (i < BIN_LENGTH) {
    result = (multiplicand[i] ^ 1) ? result : result + (multiplier << i);
    ++i;
  }

  result = (other[BIN_LENGTH - 1] ^ (*this)[BIN_LENGTH - 1])
               ? result._getTwoComplement()
               : result;

  return result;
}

QInt QInt::operator/(const QInt& other) const {
  QInt quotient =
      ((*this)[BIN_LENGTH - 1] ^ 1) ? *this : (*this)._getTwoComplement();
  QInt divisor =
      (other[BIN_LENGTH - 1] ^ 1) ? other : other._getTwoComplement();
  QInt remainder;

  size_t i = BIN_LENGTH;
  while (i > 0) {
    remainder = remainder << 1;
    remainder[0] = quotient[BIN_LENGTH - 1];
    quotient = quotient << 1;
    remainder = remainder - divisor;

    if (remainder[BIN_LENGTH - 1] == 1) {
      quotient[0] = 0;
      remainder = remainder + divisor;
    } else {
      quotient[0] = 1;
    }
    --i;
  }

  remainder =
      ((*this)[BIN_LENGTH - 1] ^ 1) ? remainder : remainder._getTwoComplement();
  quotient = (other[BIN_LENGTH - 1] ^ (*this)[BIN_LENGTH - 1])
                 ? quotient._getTwoComplement()
                 : quotient;

  return quotient;
}

QInt QInt::operator%(const QInt& other) const {
 QInt quotient =
      ((*this)[BIN_LENGTH - 1] ^ 1) ? *this : (*this)._getTwoComplement();
  QInt divisor =
      (other[BIN_LENGTH - 1] ^ 1) ? other : other._getTwoComplement();
  QInt remainder;

  size_t i = BIN_LENGTH;
  while (i > 0) {
    remainder = remainder << 1;
    remainder[0] = quotient[BIN_LENGTH - 1];
    quotient = quotient << 1;
    remainder = remainder - divisor;

    if (remainder[BIN_LENGTH - 1] == 1) {
      quotient[0] = 0;
      remainder = remainder + divisor;
    } else {
      quotient[0] = 1;
    }
    --i;
  }

  remainder =
      ((*this)[BIN_LENGTH - 1] ^ 1) ? remainder : remainder._getTwoComplement();
  quotient = (other[BIN_LENGTH - 1] ^ (*this)[BIN_LENGTH - 1])
                 ? quotient._getTwoComplement()
                 : quotient;

  return remainder;
}

bool QInt::operator<(const QInt& other) const {
  if (*this == other) {
    return false;
  }

  if ((*this)[BIN_LENGTH - 1] ^ other[BIN_LENGTH - 1]) {
    return other[BIN_LENGTH - 1] ^ 1;

  } else {
    for (int i = BIN_LENGTH - 2; i >= 0; i--) {
      if ((*this)[i] ^ other[i]) return other[i];
    }
  }

  throw std::runtime_error("Cannot return value");
}

bool QInt::operator>(const QInt& other) const {
  return other < *this;
}

bool QInt::operator<=(const QInt& other) const {
  return (*this < other) || (*this == other);
}

bool QInt::operator>=(const QInt& other) const {
  return (*this > other) || (*this == other);
}

bool QInt::operator==(const QInt& other) const {
  return (*this)._bin == other._bin;
}

bool QInt::operator!=(const QInt& other) const {
  return (*this)._bin != other._bin;
}

QInt QInt::operator<<(size_t pos) const {
  QInt result;
  result._bin = (*this)._bin << pos;
  return result;
}

QInt QInt::operator>>(size_t pos) const {
  QInt result;
  result._bin = (*this)._bin >> pos;
  return result;
}

QInt QInt::operator&(const QInt& other) const {
  QInt result;
  result._bin = (*this)._bin & other._bin;
  return result;
}

QInt QInt::operator|(const QInt& other) const {
  QInt result;
  result._bin = (*this)._bin | other._bin;
  return result;
}

QInt QInt::operator^(const QInt& other) const {
  QInt result;
  result._bin = (*this)._bin ^ other._bin;
  return result;
}

QInt QInt::operator~() const {
  QInt result;
  result._bin = ~(*this)._bin;
  return result;
}

QInt QInt::rol(size_t pos) const {
  QInt result = *this;

  size_t i = 0;
  while (i < pos) {
    result = result << 1;
    result[0] = (*this)[BIN_LENGTH - 1 - i];
    ++i;
  }

  return result;
}

QInt QInt::ror(size_t pos) const {
  QInt result = *this;

  size_t i = 0;
  while (i < pos) {
    result = result >> 1;
    result[BIN_LENGTH - 1] = (*this)[0 + i];
    ++i;
  }

  return result;
}

std::string QInt::getContent() const { return (*this)._bin.to_string(); }

QInt QInt::_getTwoComplement() const { return QInt("1", 2) + ~*this; }

std::string QInt::hexToBin(const std::string& hexStr) {
  std::bitset<4> bchCarriers[HEX_LENGTH];     // Binary-coded hexadecimal blocks (4-bit each)
  std::string binStr = "";                    // Result string
  std::map<char, size_t> hexMap = 
      Utility::getMap(HEX_MAP, 16);

  size_t i = 0;
  while (i < HEX_LENGTH) {
    bchCarriers[i] = std::bitset<4>(      // Find numbers associate with keys and
        hexMap.find(hexStr[i])->second);  // convert them to 4-bit binary

    binStr += bchCarriers[i].to_string();
    ++i;
  }

  return binStr;
}

std::string QInt::binToHex(const std::string& binStr) {
  std::bitset<4> bchCarriers[HEX_LENGTH];  // Binary-coded hexadecimal blocks (4-bit each)
  QInt bin(binStr, 2);                     // Store binary number
  std::string hexStr = "";                 // Result string

  size_t i = 0;
  while (i < HEX_LENGTH) {
    for (size_t j = 0; j < 4; ++j) {         // Shift every 4-bit block and "tempResult" 4 times
      bchCarriers[i] = bchCarriers[i] << 1;  // concurrently as they are the same binary
      bchCarriers[i][0] = bin[BIN_LENGTH - 1];
      bin = bin << 1;
    }
    ++i;
  }

  i = 0;
  while (i < HEX_LENGTH) {                   // Map every 4-bit block to every hex character
    hexStr += HEX_MAP[bchCarriers[i].to_ulong()];
    ++i;
  }

  return hexStr;
}

// Reverse double dabble algorithm
std::string QInt::decToBin(const std::string& decStr) {
  std::bitset<4> bcdCarriers[DEC_LENGTH];          // Binary-code decimal blocks (4-bit each)
  std::bitset<1> bcdFirstBitCarriers[DEC_LENGTH];  // Store the first bit of 4-bit blocks
  QInt bin;                                        // Store temporary binary number
  std::string binStr = "";                         // Result string
  std::map<char, size_t> decMap = 
      Utility::getMap(DEC_MAP, 10);

  size_t i = 0;
  while (i < DEC_LENGTH) {
    bcdCarriers[i] = std::bitset<4>(           // Find numbers associate with keys and
        decMap.find(decStr[i + 1])->second);   // convert them to 4-bit binary
    ++i;
  }

  i = 0;
  while (i < BIN_LENGTH) {                    // Shift "tempResult" and all 4-bit blocks as one
    for (int j = DEC_LENGTH - 1; j >= 0; --j) {
      bcdFirstBitCarriers[j][0] = bcdCarriers[j][0];
    }

    bin = bin >> 1;
    bin[BIN_LENGTH - 1] = bcdFirstBitCarriers[DEC_LENGTH - 1][0];

    for (int j = DEC_LENGTH - 1; j > 0; --j) {  // Shift every 4-bit block
      bcdCarriers[j] = bcdCarriers[j] >> 1;
      bcdCarriers[j][3] = bcdFirstBitCarriers[j - 1][0];

      if (bcdCarriers[j].to_ulong() > 7         // Subtract 3 to every block that
          && i != BIN_LENGTH - 1) {             // is bigger than 7 to ensure
        bcdCarriers[j] =                        // they are in base 10
            Utility::subtract(bcdCarriers[j], 3);  
      }
    }

    bcdCarriers[0] = bcdCarriers[0] >> 1;
    ++i;
  }

  bin = (decStr[0] == '-') ? bin._getTwoComplement() : bin;
  binStr = bin._bin.to_string();

  return binStr;
}

// Double dabble algorithm
std::string QInt::binToDec(const std::string& binStr) {
  std::bitset<4> bcdCarriers[DEC_LENGTH];          // Binary-coded decimal blocks (4-bit each)
  std::bitset<1> bcdFirstBitCarriers[DEC_LENGTH];  // Store the first bit of 4-bit blocks
  std::string decStr =                                   // Initialize with empty string
      (QInt(binStr, 2)[BIN_LENGTH - 1] ^ 1) ? "" : "-";  // (with sign if the first bit is 1)

  QInt bin;                                        // Store binary number
  if (QInt(binStr, 2) != QInt("0", 2) &&           // Return number cannot be calculated
      (QInt(binStr, 2) << 1) == QInt("0", 2)) {
    return INT128_MIN;  

  } else {                                         // Initialize with "bin" (with 2's complement
    bin =                                          // if the first bit is 1)
        (QInt(binStr, 2)[BIN_LENGTH - 1] ^ 1)
            ? QInt(binStr, 2) << 1
            : QInt(binStr, 2)._getTwoComplement() << 1;
  }

  size_t i = 1;
  while (i < BIN_LENGTH) {                                // Shift "tempResult" and all 4-bit blocks as one
    for (int j = DEC_LENGTH - 2; j >= 0; --j) {  
      bcdFirstBitCarriers[j][0] = bcdCarriers[j + 1][3];  // Store the first bit of "tempResult" and every block
    }

    bcdFirstBitCarriers[DEC_LENGTH - 1][0] = bin[BIN_LENGTH - 1];
    bin = bin << 1;

    for (int j = DEC_LENGTH - 1; j > 0; --j) {  // Shift every 4-bit block
      bcdCarriers[j] = bcdCarriers[j] << 1;
      bcdCarriers[j][0] = bcdFirstBitCarriers[j][0];

      if (bcdCarriers[j].to_ulong() > 4         // Add 3 to every block that
          && i != BIN_LENGTH - 1) {             // is bigger than 4 to ensure
        bcdCarriers[j] =                        // they are in base 10
            Utility::add(bcdCarriers[j], 3);  
      }
    }
    ++i;
  }

  i = 0;
  while (i < DEC_LENGTH) {  // Map every 4-bit block to every dec character
    decStr += DEC_MAP[bcdCarriers[i].to_ulong()];
    ++i;
  }

  return decStr;
}
}  // namespace int128
}  // namespace native
