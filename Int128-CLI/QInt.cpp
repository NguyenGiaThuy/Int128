#include "QInt.h"

namespace CLI {
namespace Int128 {
QInt::QInt() : ManagedInt128(new native::int128::QInt()) {}

QInt::QInt(System::String ^ decStr)
    : ManagedInt128(
          new native::int128::QInt((systemStringToStdString(decStr)), 10)) {}

QInt ^ QInt::operator=(QInt ^ other) {
  *this->_instance = *other->_instance;
  return this;
}

QInt ^ QInt::operator+(QInt ^ a, QInt ^ b) {
  QInt ^ result = gcnew QInt;
  *result->_instance = *a->_instance + *b->_instance;

  QInt ^ qint0 = gcnew QInt;
  if ((a > qint0 && b < qint0 && result < qint0) ||
      (a < qint0 && b > qint0 && result > qint0)) {
    throw gcnew System::OverflowException("Overflow");
  }

  return result;
}

QInt ^ QInt::operator-(QInt ^ a, QInt ^ b) {
  QInt ^ result = gcnew QInt;
  *result->_instance = *a->_instance - *b->_instance;

  QInt ^ qint0 = gcnew QInt;
  if ((a > qint0 && b < qint0 && result < qint0) ||
      (a < qint0 && b > qint0 && result > qint0)) {
    throw gcnew System::OverflowException("Overflow");
  }

  return result;
}

QInt ^ QInt::operator*(QInt ^ a, QInt ^ b) {
  QInt ^ result = gcnew QInt;
  *result->_instance = *a->_instance * *b->_instance;

  QInt ^ qint0 = gcnew QInt;

  if ((a > qint0 && b > qint0 && result < qint0) ||
      (a < qint0 && b < qint0 && result < qint0) ||
      (a < qint0 && b > qint0 && result > qint0) ||
      (a > qint0 && b < qint0 && result > qint0)) {
    throw gcnew System::OverflowException("Overflow");
  }

  return result;
}

QInt ^ QInt::operator/(QInt ^ a, QInt ^ b) {
  if (b == gcnew QInt()) {
    throw gcnew System::DivideByZeroException("Attempted to divide by zero");
  }

  QInt ^ result = gcnew QInt;
  *result->_instance = *a->_instance / *b->_instance;
  return result;
}

QInt ^ QInt::operator%(QInt ^ a, QInt ^ b) {
  if (b == gcnew QInt()) {
    throw gcnew System::DivideByZeroException("Attempted to divide by zero");
  }

  QInt ^ result = gcnew QInt;
  *result->_instance = *a->_instance % *b->_instance;
  return result;
}

bool QInt::operator<(QInt ^ a, QInt ^ b) {
  return *a->_instance < *b->_instance;
}

bool QInt::operator>(QInt ^ a, QInt ^ b) {
  return *a->_instance > *b->_instance;
}

bool QInt::operator<=(QInt ^ a, QInt ^ b) {
  return *a->_instance <= *b->_instance;
}

bool QInt::operator>=(QInt ^ a, QInt ^ b) {
  return *a->_instance >= *b->_instance;
}

bool QInt::operator==(QInt ^ a, QInt ^ b) {
  return *a->_instance == *b->_instance;
}

bool QInt::operator!=(QInt ^ a, QInt ^ b) {
  return *a->_instance != *b->_instance;
}

QInt ^ QInt::operator<<(QInt ^ a, size_t pos) {
  QInt ^ result = gcnew QInt;
  *result->_instance = *a->_instance << pos;
  return result;
}

QInt ^ QInt::operator>>(QInt ^ a, size_t pos) {
  QInt ^ result = gcnew QInt;
  *result->_instance = *a->_instance >> pos;
  return result;
}

QInt ^ QInt::operator&(QInt ^ a, QInt ^ b) {
  QInt ^ result = gcnew QInt;
  *result->_instance = *a->_instance & *b->_instance;
  return result;
}

QInt ^ QInt::operator|(QInt ^ a, QInt ^ b) {
  QInt ^ result = gcnew QInt;
  *result->_instance = *a->_instance | *b->_instance;
  return result;
}

QInt ^ QInt::operator^(QInt ^ a, QInt ^ b) {
  QInt ^ result = gcnew QInt;
  *result->_instance = *a->_instance ^ *b->_instance;
  return result;
}

QInt ^ QInt::operator~(QInt ^ a) {
  QInt ^ result = gcnew QInt;
  *result->_instance = ~*a->_instance;
  return result;
}

QInt ^ QInt::rol(size_t pos) {
  QInt ^ result = gcnew QInt;
  *result->_instance = (*this->_instance).rol(pos);
  return result;
}

QInt ^ QInt::ror(size_t pos) {
  QInt ^ result = gcnew QInt;
  *result->_instance = (*this->_instance).ror(pos);
  return result;
}

System::String ^ QInt::DecToBin(System::String ^ decStr) {
  System::String ^ resultStr = gcnew System::String(
      native::int128::QInt::decToBin(native::int128::Utility::formatDecStr(
                                         systemStringToStdString(decStr)))
          .c_str());

  if (resultStr->Contains("1")) {
    resultStr = resultStr->TrimStart('0');
  } else {
    resultStr = resultStr->Remove(1);
  }

  return resultStr;
}

System::String ^ QInt::BinToDec(System::String ^ binStr) {
  System::String ^ resultStr = gcnew System::String(
      native::int128::QInt::binToDec(systemStringToStdString(binStr)).c_str());

  if (resultStr[0] == '-') {
    resultStr = resultStr->Remove(1, resultStr->IndexOfAny(_decTokens) - 1);
  } else {
    if (resultStr->IndexOfAny(_decTokens) != -1) {
      resultStr = resultStr->TrimStart('0');
    } else {
      resultStr = resultStr->Remove(1);
    }
  }

  return resultStr;
}

System::String ^ QInt::HexToBin(System::String ^ hexStr) {
  System::String ^ resultStr = gcnew System::String(
      native::int128::QInt::hexToBin(native::int128::Utility::formatHexStr(
                                         systemStringToStdString(hexStr)))
          .c_str());

  if (resultStr->Contains("1")) {
    resultStr = resultStr->TrimStart('0');
  } else {
    resultStr = resultStr->Remove(1);
  }

  return resultStr;
}

System::String ^ QInt::BinToHex(System::String ^ binStr) {
  System::String ^ resultStr = gcnew System::String(
      native::int128::QInt::binToHex(systemStringToStdString(binStr)).c_str());

  if (resultStr->IndexOfAny(_hexTokens) != -1) {
    resultStr = resultStr->TrimStart('0');
  } else {
    resultStr = resultStr->Remove(1);
  }

  return resultStr;
}
}  // namespace Int128
}  // namespace CLI