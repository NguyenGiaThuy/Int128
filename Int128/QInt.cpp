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

  return result;
}

QInt QInt::operator-(const QInt& other) const {
  QInt result;
  result = *this + other._getTwoComplement();
  return result;
}

QInt QInt::operator*(const QInt& other) const {
  QInt result;

  size_t i = 0;
  while (i < MAX_LENGTH) {
    result = (other[i] ^ 1) ? result : result + (*this << i);
    ++i;
  }

  return result;
}

QInt QInt::operator/(const QInt& other) const {
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
  QInt temp = *this - other;
  return temp[MAX_LENGTH - 1] ^ 0;
}

bool QInt::operator>(const QInt& other) const {
  QInt temp = *this - other;
  return (temp[MAX_LENGTH - 1] ^ 1) && (*this != other);
}

bool QInt::operator<=(const QInt& other) const {
  QInt temp = *this - other;
  return (temp[MAX_LENGTH - 1] ^ 0) || (*this == other);
}

bool QInt::operator>=(const QInt& other) const {
  QInt temp = *this - other;
  return temp[MAX_LENGTH - 1] ^ 1;
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
    result[0] = (*this)[MAX_LENGTH - 1];
    ++i;
  }

  return result;
}

QInt QInt::ror(size_t position) const {
  QInt result = *this;

  size_t i = 0;
  while (i < position) {
    result = result >> 1;
    result[MAX_LENGTH - 1] = (*this)[0];
    ++i;
  }

  return result;
}

std::string QInt::getContent() const { return (*this)._bits.to_string(); }

QInt QInt::_getTwoComplement() const { return QInt("1", 10) + ~(*this); }

std::string QInt::hexToBin(const std::string& hex) {
  constexpr size_t size = MAX_LENGTH / 4;  // Numbers of 4-bit blocks

  std::string tempHex;
  if (tempHex[0] >= 'a' || tempHex[0] >= '8') {
    tempHex = std::string(size, 'f');  // Assign leading 'f'
  } else {
    tempHex = std::string(size, '0');  // Assign leading zero
  }
  tempHex.replace(             // Copy "hex"
      tempHex.size() - hex.size(), hex.size(), hex); 
  tempHex = toUpper(tempHex);  // Transform all characters to lower case

  std::bitset<4> carriers[size];           // Block container
  std::string bin = "";                    // Result string

  std::map<char, size_t> hexMap;           // Key-value = char-number
  for (size_t i = 0; i < 16; ++i) {
    hexMap[HEX_MAP[i]] = i;
  }

  size_t i = 0;
  while (i < size) {
    carriers[i] = std::bitset<4>(          // Find numbers associate with keys and
        hexMap.find(tempHex[i])->second);  // convert them to 4-bit binary

    bin += carriers[i].to_string();
    ++i;
  }

  return bin;
}

std::string QInt::binToHex(const std::string& bin) {
  constexpr size_t size = MAX_LENGTH / 4;  // Numbers of 4-bit blocks
  std::bitset<4> carriers[size];           // Block container
  std::string hex = "";                    // Result string
  QInt temp = QInt(bin, 2);                // Store temporary values of input "bin"

  size_t i = 0;
  while (i < size) {
    for (size_t j = 0; j < 4; ++j) {   // Shift every 4-bit block and "temp" 4 times
      carriers[i] = carriers[i] << 1;  // concurrently as they are the same binary
      carriers[i][0] = temp[MAX_LENGTH - 1];
      temp = temp << 1;
    }
    ++i;
  }

  i = 0;
  while (i < size) {  // Map every 4-bit block to every hex character
    hex += HEX_MAP[carriers[i].to_ulong()];
    ++i;
  }

  return hex;
}

// Reverse double dabble algorithm
std::string QInt::decToBin(const std::string& dec) {
  constexpr size_t size = static_cast<size_t>(  // Numbers of 4-bit blocks
      customCeil(static_cast<float>(MAX_LENGTH) / 3));

  std::string tempDec(size, '0');
  if (dec[0] != '-') {  // Copy "dec" and assign leading zeros
    tempDec.replace(tempDec.size() - dec.size(), dec.size(), dec);
  } else {              // Copy "dec" without sign and assign leading zeros
    tempDec.replace(tempDec.begin() + (tempDec.size() - dec.size() + 1),
                    tempDec.end(), dec.begin() + 1, dec.end());
  }

  std::bitset<4> carriers[size];      // Block container
  std::bitset<1> tempCarriers[size];  // Store the first bit of 4-bit blocks
  std::string bin = "";               // Result string
  QInt temp;                          // Store temporary binary result

  std::map<char, size_t> decMap;      // Key-value = char-number
  for (size_t i = 0; i < 10; ++i) {
    decMap[DEC_MAP[i]] = i;
  }

  size_t i = 0;
  while (i < size) {
    carriers[i] = std::bitset<4>(          // Find numbers associate with keys and
        decMap.find(tempDec[i])->second);  // convert them to 4-bit binary

    ++i;
  }

  i = 0;
  while (i < MAX_LENGTH) {  // Shift "temp" and all 4-bit blocks as one
    for (int j = size - 1; j >= 0; --j) {
      tempCarriers[j][0] = carriers[j][0];
    }

    temp = temp >> 1;
    temp[MAX_LENGTH - 1] = tempCarriers[size - 1][0];

    for (int j = size - 1; j > 0; --j) {  // Shift every 4-bit block
      carriers[j] = carriers[j] >> 1;
      carriers[j][3] = tempCarriers[j - 1][0];
      if (carriers[j].to_ulong() > 7      // Subtract 3 to every block that
          && i != MAX_LENGTH - 1) {       // is bigger than 7 to ensure
        carriers[j] = carriers[j] - 3;    // they are in base 10
      }
    }
    carriers[0] = carriers[0] >> 1;
    ++i;
  }

  if (dec[0] == '-') {
    temp = temp._getTwoComplement();
  }
  bin = temp._bits.to_string();

  return bin;
}

// Double dabble algorithm
std::string QInt::binToDec(const std::string& bin) {
  constexpr size_t size = static_cast<size_t>(  // Numbers of 4-bit blocks
      customCeil(static_cast<float>(MAX_LENGTH) / 3));

  std::bitset<4> carriers[size];              // Block container
  std::bitset<1> tempCarriers[size];          // Store the first bit of 4-bit blocks
  std::string dec =                           // Initialize with empty string
      (QInt(bin, 2)[MAX_LENGTH - 1] ^ 1)  // (with sign if the first bit is 1)
          ? ""  
          : "-";

  QInt temp;
  if (QInt(bin, 2) != QInt("0", 2) &&
      (QInt(bin, 2) << 1) == QInt("0", 2)) {
    return INT128_MIN;  // Number cannot be calculated
  } else {
    temp =                                  // Initialize with "bin" (with 2's complement
        (QInt(bin, 2)[MAX_LENGTH - 1] ^ 1)  // if the first bit is 1)
            ? QInt(bin, 2) << 1
            : QInt(bin, 2)._getTwoComplement() << 1;
  }

  size_t i = 1;
  while (i < MAX_LENGTH) {                 // Shift "temp" and all 4-bit blocks as one
    for (int j = size - 2; j >= 0; --j) {  // Store the first bit of "temp" and every block
      tempCarriers[j][0] = carriers[j + 1][3];
    }

    tempCarriers[size - 1][0] = temp[MAX_LENGTH - 1];
    temp = temp << 1;

    for (int j = size - 1; j > 0; --j) {  // Shift every 4-bit block
      carriers[j] = carriers[j] << 1;
      carriers[j][0] = tempCarriers[j][0];
      if (carriers[j].to_ulong() > 4      // Add 3 to every block that
          && i != MAX_LENGTH - 1) {       // is bigger than 4 to ensure
        carriers[j] = carriers[j] + 3;    // they are in base 10
      }
    }
    ++i;
  }

  i = 0;
  while (i < size) {  // Map every 4-bit block to every dec character
    dec += DEC_MAP[carriers[i].to_ulong()];
    ++i;
  }

  return dec;
}
}  // namespace Int128
}  // namespace Native
