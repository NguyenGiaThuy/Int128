#include "QInt.h"

namespace CLI {
namespace Int128 {
QInt::QInt() : ManagedInt128(new Native::Int128::QInt()) {}

QInt::QInt(System::String ^ dec)
    : ManagedInt128(
          new Native::Int128::QInt(systemStringToStdString(dec), 10)) {}

QInt ^ QInt::operator=(QInt ^ other) {
  *this->_instance = *other->_instance;
  return this;
}

QInt ^ QInt::operator+(QInt ^ a, QInt ^ b) {
  QInt ^ result = gcnew QInt;
  *result->_instance = *a->_instance + *b->_instance;
  return result;
}

QInt ^ QInt::operator-(QInt ^ a, QInt ^ b) {
  QInt ^ result = gcnew QInt;
  *result->_instance = *a->_instance - *b->_instance;
  return result;
}

QInt ^ QInt::operator*(QInt ^ a, QInt ^ b) {
  QInt ^ result = gcnew QInt;
  *result->_instance = *a->_instance * *b->_instance;
  return result;
}

QInt ^ QInt::operator/(QInt ^ a, QInt ^ b) {
  if (b == gcnew QInt) {
    throw gcnew System::DivideByZeroException("Attempted to divide by zero!");
  }

  QInt ^ result = gcnew QInt;
  *result->_instance = *a->_instance / *b->_instance;
  return result;
}

QInt ^ QInt::operator%(QInt ^ a, QInt ^ b) {
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

QInt ^ QInt::operator<<(QInt ^ a, size_t position) {
  QInt ^ result = gcnew QInt;
  *result->_instance = *a->_instance << position;
  return result;
}

QInt ^ QInt::operator>>(QInt ^ a, size_t position) {
  QInt ^ result = gcnew QInt;
  *result->_instance = *a->_instance >> position;
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

QInt ^ QInt::rol(size_t position) {
  QInt ^ result = gcnew QInt;
  (*(*result)._instance) = (*(*this)._instance).rol(position);
  return result;
}

QInt ^ QInt::ror(size_t position) {
  QInt ^ result = gcnew QInt;
  (*(*result)._instance) = (*(*this)._instance).ror(position);
  return result;
}

System::String ^ QInt::DecToBin(System::String ^ dec) {
  System::String ^ result = gcnew System::String(
      Native::Int128::QInt::decToBin(systemStringToStdString(dec)).c_str());

  if (result->Contains("1")) {
    result = result->TrimStart('0');
  } else {
    result = result->Remove(1);
  }

  return result;
}

System::String ^ QInt::BinToDec(System::String ^ bin) {
  System::String ^ result = gcnew System::String(
      Native::Int128::QInt::binToDec(systemStringToStdString(bin)).c_str());

  if (result[0] == '-') {  // erase leading zeros
    result = result->Remove(1, result->IndexOfAny(_decTokens) - 1);
  } else {
    if (result->IndexOfAny(_decTokens) != -1) {
      result = result->TrimStart('0');
    } else {
      result = result->Remove(1);
    }
  }

  return result;
}

System::String ^ QInt::HexToBin(System::String ^ hex) {
  System::String ^ result = gcnew System::String(
      Native::Int128::QInt::hexToBin(systemStringToStdString(hex)).c_str());

  if (result->Contains("1")) {
    result = result->TrimStart('0');
  } else {
    result = result->Remove(1);
  }

  return result;
}

System::String ^ QInt::BinToHex(System::String ^ bin) {
  System::String ^ result = gcnew System::String(
      Native::Int128::QInt::binToHex(systemStringToStdString(bin)).c_str());

  if (result->IndexOfAny(_hexTokens) != -1) {
    result = result->TrimStart('0');
  } else {
    result = result->Remove(1);
  }

  return result;
}
}  // namespace Int128
}  // namespace CLI