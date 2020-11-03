#include "QInt.h"

namespace native {
namespace int128 {
QInt::QInt() { this->_bin = std::bitset<INT128_BIN_LENGTH>(); }

QInt::QInt(const std::string& str, int base) {
  switch (base) {
    case 2:
      this->_bin = std::bitset<INT128_BIN_LENGTH>(str);
      break;
    case 10:
      this->_bin =
          std::bitset<INT128_BIN_LENGTH>(
              QInt::decToBin(utility::formatDecStr(str, 'i')));
      break;
    case 16:
      this->_bin =
          std::bitset<INT128_BIN_LENGTH>(
              QInt::hexToBin(utility::formatHexStr(str)));
      break;
  }
}

QInt& QInt::operator=(const QInt& other) {
  this->_bin = other._bin;
  return *this;
}

std::bitset<INT128_BIN_LENGTH>::reference QInt::operator[](size_t pos) {
  return this->_bin[pos];
}

constexpr bool QInt::operator[](size_t pos) const {
  return this->_bin[pos];
}

QInt QInt::operator+(const QInt& other) const {
  QInt result;             // Store the result
  std::bitset<1> carrier;  // Store the overflow bit

  // Evaluate bit by bit of two QInts
  size_t i = 0;
  while (i < INT128_BIN_LENGTH) {
    result[i] = (*this)[i] ^ other[i] ^ carrier[0];  // Evaluate a bit at position i
    carrier = ((*this)[i] & other[i]) |              // Evaluate the overflow bit
        ((*this)[i] & carrier[0]) | 
        (other[i] & carrier[0]);
    ++i;
  }

  return result;
}

QInt QInt::operator-(const QInt& other) const {
  QInt result;
  result = *this + other.getTwoComplement();
  return result;
}

QInt QInt::operator*(const QInt& other) const {
  // Convert from signed QInts to unsigned QInts
  QInt multiplier = ((*this)[INT128_BIN_LENGTH - 1] ^ 1) ?  // Convert the multiplier
      *this : this->getTwoComplement();  
  QInt multiplicand = (other[INT128_BIN_LENGTH - 1] ^ 1) ?  // Convert the mltiplicand
      other : other.getTwoComplement();  
  QInt result;  // Store the result

  // Evaluate the result accordingly to a multiplicand bit at position i,
  // if it is 0 then do nothing, if it is 1 then add the multiplier to the
  // result, then shift left by 1 position each iteration
  size_t i = 0;
  while (i < INT128_BIN_LENGTH) {
    result = (multiplicand[i] ^ 1) ? result : result + (multiplier << i);
    ++i;
  }

  // Decide whether the result is a signed or unsigned QInt
  result = (other[INT128_BIN_LENGTH - 1] ^ (*this)[INT128_BIN_LENGTH - 1]) ?
      result.getTwoComplement() : result;

  return result;
}

QInt QInt::operator/(const QInt& other) const {
  // Convert from signed QInts to unsigned QInts
  QInt quotient = ((*this)[INT128_BIN_LENGTH - 1] ^ 1) ?  // Convert the quotient
      *this : this->getTwoComplement();
  QInt divisor = (other[INT128_BIN_LENGTH - 1] ^ 1) ?     // Convert the divisor
      other : other.getTwoComplement();
  QInt remainder;  // Store the remainder

  // ??? algorithm
  int i = INT128_BIN_LENGTH;
  while (i > 0) {
    remainder = remainder << 1;
    remainder[0] = quotient[INT128_BIN_LENGTH - 1];
    quotient = quotient << 1;
    remainder = remainder - divisor;

    if (remainder[INT128_BIN_LENGTH - 1] == 1) {
      quotient[0] = 0;
      remainder = remainder + divisor;
    } else {
      quotient[0] = 1;
    }
    --i;
  }

  // Decide whether the remainder and quotient
  // are signed or unsigned QInts
  remainder = ((*this)[INT128_BIN_LENGTH - 1] ^ 1) ? 
      remainder : remainder.getTwoComplement();
  quotient = (other[INT128_BIN_LENGTH - 1] ^ (*this)[INT128_BIN_LENGTH - 1]) ?
      quotient.getTwoComplement() : quotient;

  return quotient;
}

QInt QInt::operator%(const QInt& other) const {
  // Convert from signed QInts to unsigned QInts
  QInt quotient = ((*this)[INT128_BIN_LENGTH - 1] ^ 1) ?  // Convert the quotient 
      *this : this->getTwoComplement();
  QInt divisor = (other[INT128_BIN_LENGTH - 1] ^ 1) ?     // Convert the divisor
      other : other.getTwoComplement();
  QInt remainder;  // Store the remainder

  // Division algorithm
  // (read document for detail)
  int i = INT128_BIN_LENGTH;
  while (i > 0) {
    remainder = remainder << 1;
    remainder[0] = quotient[INT128_BIN_LENGTH - 1];
    quotient = quotient << 1;
    remainder = remainder - divisor;

    if (remainder[INT128_BIN_LENGTH - 1] == 1) {
      quotient[0] = 0;
      remainder = remainder + divisor;
    } else {
      quotient[0] = 1;
    }
    --i;
  }

  // Decide whether the remainder and quotient
  // are signed or unsigned QInts
  remainder = ((*this)[INT128_BIN_LENGTH - 1] ^ 1) ? 
      remainder : remainder.getTwoComplement();
  quotient = (other[INT128_BIN_LENGTH - 1] ^ (*this)[INT128_BIN_LENGTH - 1]) ? 
      quotient.getTwoComplement() : quotient;

  return remainder;
}

QInt QInt::exp(size_t exp) const {
  QInt result("1", 2);
  for (size_t i = 0; i < exp; i++) {
    result = result * *this;
  }
  return result;
}

bool QInt::operator<(const QInt& other) const {
  if (*this != other) {
    // Check for the two QInts are opposite or not,
    // then return value accordingly
    if ((*this)[INT128_BIN_LENGTH - 1] ^  // If both are opposite,
        other[INT128_BIN_LENGTH - 1]) {   // then return the unsigned one
      return other[INT128_BIN_LENGTH - 1] ^ 1;

    } else {  // If not, then return the one
              // with the larger significant bit
      for (int i = INT128_BIN_LENGTH - 2; i >= 0; i--) {
        if ((*this)[i] ^ other[i]) return other[i];
      }
    }
  }

  // False both QInts are equal
  return false;
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
  return this->_bin == other._bin;
}

bool QInt::operator!=(const QInt& other) const {
  return this->_bin != other._bin;
}

QInt QInt::operator<<(size_t pos) const {
  QInt result;
  result._bin = this->_bin << pos;
  return result;
}

QInt QInt::operator>>(size_t pos) const {
  QInt result;
  result._bin = this->_bin >> pos;
  return result;
}

QInt QInt::operator&(const QInt& other) const {
  QInt result;
  result._bin = this->_bin & other._bin;
  return result;
}

QInt QInt::operator|(const QInt& other) const {
  QInt result;
  result._bin = this->_bin | other._bin;
  return result;
}

QInt QInt::operator^(const QInt& other) const {
  QInt result;
  result._bin = this->_bin ^ other._bin;
  return result;
}

QInt QInt::operator~() const {
  QInt result;
  result._bin = ~this->_bin;
  return result;
}

QInt QInt::rol(size_t pos) const {
  QInt result = *this;

  size_t i = 0;
  while (i < pos) {
    result = result << 1;
    result[0] = (*this)[INT128_BIN_LENGTH - 1 - i];
    ++i;
  }

  return result;
}

QInt QInt::ror(size_t pos) const {
  QInt result = *this;

  size_t i = 0;
  while (i < pos) {
    result = result >> 1;
    result[INT128_BIN_LENGTH - 1] = (*this)[0 + i];
    ++i;
  }

  return result;
}

std::string QInt::getBin() const { return this->_bin.to_string(); }

std::string QInt::getDec() const { return binToDec(this->_bin.to_string()); }

QInt QInt::getTwoComplement() const { return QInt("1", 2) + ~*this; }

std::string QInt::hexToBin(const std::string& hexStr) {
  std::bitset<4> bchCarriers[INT128_HEX_LENGTH];  // Binary-coded hexadecimal array
  std::string binStr = "";                 // Store the result string
  std::map<char, size_t> hexMap =          // Store the hex map derived from macro        
      utility::getMap(HEX_MAP, 16);

  // Find each BCH associate with each key 
  // and convert it to binary
  size_t i = 0;
  while (i < INT128_HEX_LENGTH) {
    bchCarriers[i] = std::bitset<4>(      
        hexMap.find(hexStr[i])->second);  
    binStr += bchCarriers[i].to_string();
    ++i;
  }

  return binStr;
}

std::string QInt::binToHex(const std::string& binStr) {
  std::bitset<4> bchCarriers[INT128_HEX_LENGTH];  // Binary-coded hexadecimal array
  QInt bin(binStr, 2);                     // Store the binary number interpreted by the binStr
  std::string hexStr = "";                 // Store the result string

  // Convert every 4-bit block of the binStr
  // to a BCH and store it in the former array
  size_t i = 0;
  while (i < INT128_HEX_LENGTH) {
    for (size_t j = 0; j < 4; ++j) {         
      bchCarriers[i] = bchCarriers[i] << 1;  
      bchCarriers[i][0] = bin[INT128_BIN_LENGTH - 1];
      bin = bin << 1;
    }
    ++i;
  }

  // Copy the BCH array to the result string
  i = 0;
  while (i < INT128_HEX_LENGTH) {
    hexStr += HEX_MAP[bchCarriers[i].to_ulong()];
    ++i;
  }

  return hexStr;
}

std::string QInt::decToBin(const std::string& decStr) {
  std::bitset<4> bcdCarriers[INT128_DEC_LENGTH];          // Binary-code decimal array
  std::bitset<1> bcdFirstBitCarriers[INT128_DEC_LENGTH];  // Store the first bit of every BCD
  QInt bin;                                        // Store a temporary binary number
  std::string binStr = "";                         // Store the result string
  std::map<char, size_t> decMap =                  // Store the dec map derived from macro  
      utility::getMap(DEC_MAP, 10);

  // Find each BCD associate with each key
  // and convert it to binary
  size_t i = 0;
  while (i < INT128_DEC_LENGTH) {
    bcdCarriers[i] = std::bitset<4>(           
        decMap.find(decStr[i + 1])->second); 
    ++i;
  }

  // Reverse double dabble algorithm 
  // (read document for detail)
  i = 0;
  while (i < INT128_BIN_LENGTH) {
    for (int j = INT128_DEC_LENGTH - 1; j >= 0; --j) {
      bcdFirstBitCarriers[j][0] = bcdCarriers[j][0];
    }

    bin = bin >> 1;
    bin[INT128_BIN_LENGTH - 1] = bcdFirstBitCarriers[INT128_DEC_LENGTH - 1][0];

    for (int j = INT128_DEC_LENGTH - 1; j > 0; --j) {
      bcdCarriers[j] = bcdCarriers[j] >> 1;
      bcdCarriers[j][3] = bcdFirstBitCarriers[j - 1][0];

      if (bcdCarriers[j].to_ulong() > 7 && i != INT128_BIN_LENGTH - 1) {
        bcdCarriers[j] = utility::subtract(bcdCarriers[j], 3);
      }
    }

    bcdCarriers[0] = bcdCarriers[0] >> 1;
    ++i;
  }

  // Decide whether the decStr is signed or unsigned
  bin = (decStr[0] == '-') ? bin.getTwoComplement() : bin;
  binStr = bin._bin.to_string();

  return binStr;
}

std::string QInt::binToDec(const std::string& binStr) {
  std::bitset<4> bcdCarriers[INT128_DEC_LENGTH];               // Binary-coded decimal array
  std::bitset<1> bcdFirstBitCarriers[INT128_DEC_LENGTH];       // Store the first bit of every BCD
  std::string decStr =                                  // Store the result, initialize with 
      (QInt(binStr, 2)[INT128_BIN_LENGTH - 1] ^ 1)? "" : "-";  // empty string or with sign      
  QInt bin;                                             // Store a temporary binary number

  // Decide whether the binary string can be evaluated or not;
  // if it is min value (cannot be evaluated), then return the min macro,
  // else discard the sign bit (get 2's complement of signed binary
  // before discarding its sign bit)
  if (QInt(binStr, 2) != QInt("0", 2) &&           
      (QInt(binStr, 2) << 1) == QInt("0", 2)) {
    return INT128_MIN;  

  } else {                                        
    bin = (QInt(binStr, 2)[INT128_BIN_LENGTH - 1] ^ 1) ?
        QInt(binStr, 2) << 1 : QInt(binStr, 2).getTwoComplement() << 1;
  }

  // Double dabble algorithm
  // (read document for detail)
  size_t i = 1;
  while (i < INT128_BIN_LENGTH) {
    for (int j = INT128_DEC_LENGTH - 2; j >= 0; --j) {
      bcdFirstBitCarriers[j][0] = bcdCarriers[j + 1][3];
    }

    bcdFirstBitCarriers[INT128_DEC_LENGTH - 1][0] = bin[INT128_BIN_LENGTH - 1];
    bin = bin << 1;

    for (int j = INT128_DEC_LENGTH - 1; j >= 0; --j) {
      bcdCarriers[j] = bcdCarriers[j] << 1;
      bcdCarriers[j][0] = bcdFirstBitCarriers[j][0];

      if (bcdCarriers[j].to_ulong() > 4 && i != INT128_BIN_LENGTH - 1) {
        bcdCarriers[j] = utility::add(bcdCarriers[j], 3);
      }
    }
    ++i;
  }

  // Copy the BCD array to the result string
  i = 0;
  while (i < INT128_DEC_LENGTH) {
    decStr += DEC_MAP[bcdCarriers[i].to_ulong()];
    ++i;
  }

  // Normalize dec string (erase leading zeros)
  if (decStr[0] == '-') {
    decStr.erase(1, decStr.find_first_not_of('0', 1) - 1);
  } else {
    if (decStr.find_first_not_of('0') != std::string::npos) {
      decStr.erase(0, decStr.find_first_not_of('0'));
    } else {
      decStr.erase(decStr.begin() + 1, decStr.end());
    }
  }

  return decStr;
}
}  // namespace int128
}  // namespace native
