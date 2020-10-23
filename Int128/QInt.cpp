#include "QInt.h"

namespace Native {
namespace Int128 {
QInt::QInt() { (*this)._bits = std::bitset<MAX_LENGTH>(); }

QInt::QInt(const std::string& str, int format) {
  switch (format) {
    case 2:
      (*this)._bits = std::bitset<MAX_LENGTH>(str);
      break;
    case 10:
      (*this)._bits = std::bitset<MAX_LENGTH>(QInt::decToBin(str));
      break;
    case 16:
      (*this)._bits = std::bitset<MAX_LENGTH>(QInt::hexToBin(str));
      break;
  }
}

QInt& QInt::operator=(const QInt& other) {
  (*this)._bits = other._bits;
  return *this;
}

std::bitset<MAX_LENGTH>::reference QInt::operator[](size_t position) {
  return (*this)._bits[position];
}

constexpr bool QInt::operator[](size_t position) const {
  return (*this)._bits[position];
}

QInt QInt::operator+(const QInt& other) const {
  QInt result;
  std::bitset<1> carrier;

  size_t i = 0;
  while (i < MAX_LENGTH) {
    result[i] = (*this)[i] ^ other[i] ^ carrier[0];

    carrier = ((*this)[i] & other[i]) | ((*this)[i] & carrier[0]) |
              (other[i] & carrier[0]);
    ++i;
  }

  if ((*this > QInt() && other > QInt() && result < QInt()) ||
      (*this < QInt() && other < QInt() && result > QInt())) {
    throw std::overflow_error("Overflow");
  }

  return result;
}

QInt QInt::operator-(const QInt& other) const {
  QInt result;
  result = *this + other._getTwoComplement();
  return result;
}

QInt QInt::operator*(const QInt& other) const {
  QInt result;
  QInt multiplier = ((*this)[MAX_LENGTH - 1] ^ 1) ? *this : (*this)._getTwoComplement();
  QInt multiplicand = (other[MAX_LENGTH - 1] ^ 1) ? other : other._getTwoComplement();

  size_t i = 0;
  while (i < MAX_LENGTH) {
    result = (multiplicand[i] ^ 1) ? result : result + (multiplier << i);
    ++i;
  }

  result = (other[MAX_LENGTH - 1] ^ (*this)[MAX_LENGTH - 1])
               ? result._getTwoComplement()
               : result;

  if ((*this > QInt() && other > QInt() && result < QInt()) ||
      (*this < QInt() && other < QInt() && result > QInt())) {
    throw std::overflow_error("Overflow");
  }

  return result;
}

QInt QInt::operator/(const QInt& other) const {
  if (other == QInt()) {
    throw std::invalid_argument("Attempted to divide by zero");
  }

  QInt quotient =
      ((*this)[MAX_LENGTH - 1] ^ 1) ? *this : (*this)._getTwoComplement();

  QInt divisor =
      (other[MAX_LENGTH - 1] ^ 1) ? other : other._getTwoComplement();

  QInt remainder;

  size_t i = MAX_LENGTH;
  while (i > 0) {
    remainder = remainder << 1;
    remainder[0] = quotient[MAX_LENGTH - 1];
    quotient = quotient << 1;
    remainder = remainder - divisor;

    if (remainder[MAX_LENGTH - 1] == 1) {
      quotient[0] = 0;
      remainder = remainder + divisor;
    } else {
      quotient[0] = 1;
    }
    --i;
  }

  remainder =
      ((*this)[MAX_LENGTH - 1] ^ 1) ? remainder : remainder._getTwoComplement();

  quotient = (other[MAX_LENGTH - 1] ^ (*this)[MAX_LENGTH - 1])
                 ? quotient._getTwoComplement()
                 : quotient;

  return quotient;
}

QInt QInt::operator%(const QInt& other) const {
  if (other == QInt()) {
    throw std::invalid_argument("Attempted to divide by zero");
  }

  QInt quotient =
      ((*this)[MAX_LENGTH - 1] ^ 1) ? *this : (*this)._getTwoComplement();

  QInt divisor =
      (other[MAX_LENGTH - 1] ^ 1) ? other : other._getTwoComplement();

  QInt remainder;

  size_t i = MAX_LENGTH;
  while (i > 0) {
    remainder = remainder << 1;
    remainder[0] = quotient[MAX_LENGTH - 1];
    quotient = quotient << 1;
    remainder = remainder - divisor;

    if (remainder[MAX_LENGTH - 1] == 1) {
      quotient[0] = 0;
      remainder = remainder + divisor;
    } else {
      quotient[0] = 1;
    }
    --i;
  }

  remainder =
      ((*this)[MAX_LENGTH - 1] ^ 1) ? remainder : remainder._getTwoComplement();

  quotient = (other[MAX_LENGTH - 1] ^ (*this)[MAX_LENGTH - 1])
                 ? quotient._getTwoComplement()
                 : quotient;

  return remainder;
}

bool QInt::operator<(const QInt& other) const {
  if (*this == other) {
    return false;
  }

  if ((*this)[MAX_LENGTH - 1] ^ other[MAX_LENGTH - 1]) {
    return other[MAX_LENGTH - 1] ^ 1;
  } else {
    for (int i = MAX_LENGTH - 2; i >= 0; i--) {
      if ((*this)[i] ^ other[i]) return other[i];
    }
  }
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
  return (*this)._bits == other._bits;
}

bool QInt::operator!=(const QInt& other) const {
  return (*this)._bits != other._bits;
}

QInt QInt::operator<<(size_t position) const {
  QInt result;
  result._bits = (*this)._bits << position;
  return result;
}

QInt QInt::operator>>(size_t position) const {
  QInt result;
  result._bits = (*this)._bits >> position;
  return result;
}

QInt QInt::operator&(const QInt& other) const {
  QInt result;
  result._bits = (*this)._bits & other._bits;
  return result;
}

QInt QInt::operator|(const QInt& other) const {
  QInt result;
  result._bits = (*this)._bits | other._bits;
  return result;
}

QInt QInt::operator^(const QInt& other) const {
  QInt result;
  result._bits = (*this)._bits ^ other._bits;
  return result;
}

QInt QInt::operator~() const {
  QInt result;
  result._bits = ~((*this)._bits);
  return result;
}

QInt QInt::rol(size_t position) const {
  QInt result = *this;

  size_t i = 0;
  while (i < position) {
    result = result << 1;
    result[0] = (*this)[MAX_LENGTH - 1 - i];
    ++i;
  }

  return result;
}

QInt QInt::ror(size_t position) const {
  QInt result = *this;

  size_t i = 0;
  while (i < position) {
    result = result >> 1;
    result[MAX_LENGTH - 1] = (*this)[0 + i];
    ++i;
  }

  return result;
}

std::string QInt::getContent() const { return (*this)._bits.to_string(); }

QInt QInt::_getTwoComplement() const { return QInt("1", 2) + ~(*this); }

std::string QInt::hexToBin(const std::string& hex) {
  constexpr size_t bchSize = MAX_LENGTH / 4;  // Numbers of 4-bit blocks

  std::string clonedHex;                      // Store cloned hexadecmial string
  if (clonedHex[0] >= 'A' || 
      clonedHex[0] >= 'a' || 
      clonedHex[0] >= '8') {
    clonedHex = std::string(bchSize, 'F');  // Assign leading 'F'
  } else {
    clonedHex = std::string(bchSize, '0');  // Assign leading zero
  }
  clonedHex.replace(                     // Copy "hex"
      clonedHex.size() - hex.size(), hex.size(), hex);

  clonedHex = toUpper(clonedHex);       // Transform all characters to upper case

  std::bitset<4> bchCarriers[bchSize];  // Binary-coded hexadecimal blocks (4-bit each)
  std::string bin = "";                 // Result string

  std::map<char, size_t> hexMap;     // Key-value = char-number
  for (size_t i = 0; i < 16; ++i) {
    hexMap[HEX_MAP[i]] = i;
  }

  size_t i = 0;
  while (i < bchSize) {
    bchCarriers[i] = std::bitset<4>(         // Find numbers associate with keys and
        hexMap.find(clonedHex[i])->second);  // convert them to 4-bit binary

    bin += bchCarriers[i].to_string();
    ++i;
  }

  return bin;
}

std::string QInt::binToHex(const std::string& bin) {
  constexpr size_t bchSize = MAX_LENGTH / 4;  // Numbers of 4-bit blocks
  std::bitset<4> bchCarriers[bchSize];        // Binary-coded hexadecimal blocks (4-bit each)
  std::string hex = "";                       // Result string
  QInt tempBin = QInt(bin, 2);                // Store temporary values of input "bin"

  size_t i = 0;
  while (i < bchSize) {
    for (size_t j = 0; j < 4; ++j) {         // Shift every 4-bit block and "tempResult" 4 times
      bchCarriers[i] = bchCarriers[i] << 1;  // concurrently as they are the same binary
      bchCarriers[i][0] = tempBin[MAX_LENGTH - 1];
      tempBin = tempBin << 1;
    }
    ++i;
  }

  i = 0;
  while (i < bchSize) {  // Map every 4-bit block to every hex character
    hex += HEX_MAP[bchCarriers[i].to_ulong()];
    ++i;
  }

  return hex;
}

// Reverse double dabble algorithm
std::string QInt::decToBin(const std::string& dec) {
  constexpr size_t bcdSize = static_cast<size_t>(  // Numbers of 4-bit blocks
      customCeil(static_cast<float>(MAX_LENGTH) / 3));

  std::string clonedDec(bcdSize, '0');  // Store cloned decimal string
  if (dec[0] != '-') {                  // Copy "dec" and assign leading zeros
    clonedDec.replace(clonedDec.size() - dec.size(), dec.size(), dec);
  } else {                              // Copy "dec" without sign and assign leading zeros
    clonedDec.replace(clonedDec.begin() + (clonedDec.size() - dec.size() + 1),
                    clonedDec.end(), dec.begin() + 1, dec.end());
  }

  std::bitset<4> bcdCarriers[bcdSize];          // Binary-code decimal blocks (4-bit each)
  std::bitset<1> bcdFirstBitCarriers[bcdSize];  // Store the first bit of 4-bit blocks
  std::string bin = "";                         // Result string
  QInt tempResult;                              // Store temporary binary result

  std::map<char, size_t> decMap;                // Key-value = char-number
  for (size_t i = 0; i < 10; ++i) {
    decMap[DEC_MAP[i]] = i;
  }

  size_t i = 0;
  while (i < bcdSize) {
    bcdCarriers[i] = std::bitset<4>(          // Find numbers associate with keys and
        decMap.find(clonedDec[i])->second);   // convert them to 4-bit binary

    ++i;
  }

  i = 0;
  while (i < MAX_LENGTH) {  // Shift "tempResult" and all 4-bit blocks as one
    for (int j = bcdSize - 1; j >= 0; --j) {
      bcdFirstBitCarriers[j][0] = bcdCarriers[j][0];
    }

    tempResult = tempResult >> 1;
    tempResult[MAX_LENGTH - 1] = bcdFirstBitCarriers[bcdSize - 1][0];

    for (int j = bcdSize - 1; j > 0; --j) {  // Shift every 4-bit block
      bcdCarriers[j] = bcdCarriers[j] >> 1;
      bcdCarriers[j][3] = bcdFirstBitCarriers[j - 1][0];
      if (bcdCarriers[j].to_ulong() > 7       // Subtract 3 to every block that
          && i != MAX_LENGTH - 1) {           // is bigger than 7 to ensure
        bcdCarriers[j] = bcdCarriers[j] - 3;  // they are in base 10
      }
    }
    bcdCarriers[0] = bcdCarriers[0] >> 1;
    ++i;
  }

  if (dec[0] == '-') {
    tempResult = tempResult._getTwoComplement();
  }
  bin = tempResult._bits.to_string();

  return bin;
}

// Double dabble algorithm
std::string QInt::binToDec(const std::string& bin) {
  constexpr size_t bcdSize = static_cast<size_t>(  // Numbers of 4-bit blocks
      customCeil(static_cast<float>(MAX_LENGTH) / 3));

  std::bitset<4> bcdCarriers[bcdSize];          // Binary-coded decimal blocks (4-bit each)
  std::bitset<1> bcdFirstBitCarriers[bcdSize];  // Store the first bit of 4-bit blocks
  std::string dec =                                   // Initialize with empty string
      (QInt(bin, 2)[MAX_LENGTH - 1] ^ 1) ? "" : "-";  // (with sign if the first bit is 1)

  QInt tempResult;                          // Store temporary result
  if (QInt(bin, 2) != QInt("0", 2) && (QInt(bin, 2) << 1) == QInt("0", 2)) {
    return INT128_MIN;                      // Number cannot be calculated
  } else {
    tempResult =                            // Initialize with "bin" (with 2's complement
        (QInt(bin, 2)[MAX_LENGTH - 1] ^ 1)  // if the first bit is 1)
            ? QInt(bin, 2) << 1
            : QInt(bin, 2)._getTwoComplement() << 1;
  }

  size_t i = 1;
  while (i < MAX_LENGTH) {  // Shift "tempResult" and all 4-bit blocks as one
    for (int j = bcdSize - 2; j >= 0; --j) {  
      bcdFirstBitCarriers[j][0] = bcdCarriers[j + 1][3];  // Store the first bit of "tempResult" and every block
    }

    bcdFirstBitCarriers[bcdSize - 1][0] = tempResult[MAX_LENGTH - 1];
    tempResult = tempResult << 1;

    for (int j = bcdSize - 1; j > 0; --j) {  // Shift every 4-bit block
      bcdCarriers[j] = bcdCarriers[j] << 1;
      bcdCarriers[j][0] = bcdFirstBitCarriers[j][0];
      if (bcdCarriers[j].to_ulong() > 4       // Add 3 to every block that
          && i != MAX_LENGTH - 1) {           // is bigger than 4 to ensure
        bcdCarriers[j] = bcdCarriers[j] + 3;  // they are in base 10
      }
    }
    ++i;
  }

  i = 0;
  while (i < bcdSize) {  // Map every 4-bit block to every dec character
    dec += DEC_MAP[bcdCarriers[i].to_ulong()];
    ++i;
  }

  return dec;
}
}  // namespace Int128
}  // namespace Native
