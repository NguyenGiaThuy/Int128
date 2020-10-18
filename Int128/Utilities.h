#pragma once
#include <algorithm>
#include <bitset>
#include <cctype>
#include <map>
#include <string>

namespace {
std::string toUpper(const std::string& str) {
  std::string result = str;
  std::transform(result.begin(), result.end(), result.begin(),
                 [](unsigned char c) { return std::toupper(c); });
  return result;
}

constexpr size_t customCeil(float dec) {
  size_t result = static_cast<size_t>(dec);
  float temp = dec - static_cast<float>(result);
  return (temp > 0) ? result + 1 : result;
}

std::bitset<4> operator+(const std::bitset<4>& first,
                         const std::bitset<4>& second) {
  std::bitset<4> result;
  std::bitset<1> carrier;

  size_t i = 0;
  while (i < 4) {
    result[i] = first[i] ^ second[i] ^ carrier[0];
    carrier = (first[i] & second[i]) | (first[i] & carrier[0]) |
              (second[i] & carrier[0]);
    ++i;
  }

  return result;
}

std::bitset<4> getTwoComplement(const std::bitset<4>& bitset) {
  return 1 + ~bitset;
}

std::bitset<4> operator-(const std::bitset<4>& first,
                         const std::bitset<4>& second) {
  std::bitset<4> result;
  result = first + getTwoComplement(second);
  return result;
}
}  // namespace
