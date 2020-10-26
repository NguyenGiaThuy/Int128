#pragma once

namespace CLI {
namespace Int128 {
public
ref class ManagedInt128 {
 public:
  ManagedInt128(native::int128::QInt* instance) : _instance(instance) {}

  ~ManagedInt128() {
    if (_instance != nullptr) {
      delete _instance;
    }
  }

  !ManagedInt128() {
    if (_instance != nullptr) {
      delete _instance;
    }
  }

 protected:
  native::int128::QInt* _instance;

 protected:
  static std::string systemStringToStdString(System::String ^ systemString) {
    std::string stdString(
        (const char*)
            System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(
                systemString)
                .ToPointer());
    return stdString;
  }
};
}  // namespace Int128
}  // namespace CLI
