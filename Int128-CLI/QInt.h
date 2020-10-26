#pragma once
#include "../Int128/Int128.h"
#include "ManagedInt128.h"

namespace CLI {
namespace Int128 {
public
ref class QInt : public ManagedInt128 {
 public:
  QInt();
  QInt(System::String ^);
  QInt ^ operator=(QInt ^);

  QInt ^ rol(size_t);
  QInt ^ ror(size_t);

 public:
  static QInt ^ operator+(QInt ^, QInt ^);
  static QInt ^ operator-(QInt ^, QInt ^);
  static QInt ^ operator*(QInt ^, QInt ^);
  static QInt ^ operator/(QInt ^, QInt ^);
  static QInt ^ operator%(QInt ^, QInt ^);

  static bool operator<(QInt ^, QInt ^);
  static bool operator>(QInt ^, QInt ^);
  static bool operator<=(QInt ^, QInt ^);
  static bool operator>=(QInt ^, QInt ^);
  static bool operator==(QInt ^, QInt ^);
  static bool operator!=(QInt ^, QInt ^);

  static QInt ^ operator<<(QInt ^, size_t);
  static QInt ^ operator>>(QInt ^, size_t);
  static QInt ^ operator&(QInt ^, QInt ^);
  static QInt ^ operator|(QInt ^, QInt ^);
  static QInt ^ operator^(QInt ^, QInt ^);
  static QInt ^ operator~(QInt ^);

  static System::String ^ DecToBin(System::String ^);
  static System::String ^ BinToDec(System::String ^);

  static System::String ^ HexToBin(System::String ^);
  static System::String ^ BinToHex(System::String ^);

 public:
  property System::String ^ Content {
   public:
    System::String ^ get() {
      return BinToDec(gcnew System::String(_instance->getBin().c_str()));
    }
  };

 private:
  static array<wchar_t> ^ _decTokens =
      gcnew array<wchar_t>{'1', '2', '3', '4', '5', '6', '7', '8', '9'};
  static array<wchar_t> ^ _hexTokens =
      gcnew array<wchar_t>{'1', '2', '3', '4', '5', '6', '7', '8',
                           '9', 'A', 'B', 'C', 'D', 'E', 'F'};

 public:
  static property System::String ^ MaxSize {
   public:
    System::String ^ get() { return gcnew System::String(INT128_MAX); }
  };

  static property System::String ^ MinSize {
   public:
    System::String ^ get() { return gcnew System::String(INT128_MIN); }
  };
};
}  // namespace Int128
}  // namespace CLI
