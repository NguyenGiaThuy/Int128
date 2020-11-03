#include "QFloat.h"

namespace native {
namespace float128 {
std::string QFloat::getExponentStr(const std::string& decStr) {
  std::string exponentStr = decStr.substr(decStr.find('e') + 1);
  return exponentStr;
}

std::string QFloat::getIntegralStr(const std::string& decStr) {
  std::string integralStr = (decStr[0] == '-')
                                ? decStr.substr(1, decStr.find('.') - 1)
                                : decStr.substr(0, decStr.find('.'));
  return integralStr;
}

std::string QFloat::getFractionalStr(const std::string& decStr) {
  std::string fractionalStr = decStr.substr(decStr.find('.') + 1);
  fractionalStr = fractionalStr.erase(fractionalStr.find('e'));
  return fractionalStr;
}

std::string QFloat::exponentDecToBin(const std::string& exponentDecStr) {
  std::bitset<15> expBits(std::stol(exponentDecStr));
  return expBits.to_string();
}

std::string QFloat::integralDecToBin(const std::string& integralDecStr) {
  std::string integralBinStr =
      int128::QInt::decToBin(utility::formatDecStr(integralDecStr, 'i'));
  integralBinStr =
      (integralBinStr.find_first_not_of('0') == std::string::npos)
          ? integralBinStr.substr(integralBinStr.find_last_of('0'))
          : integralBinStr.substr(integralBinStr.find_first_not_of('0'));
  return integralBinStr;
}

std::string QFloat::fractionalDecToBin(const std::string& fractionalDecStr) {
  // Store the fractional part as decimal number
  int128::QInt fractqint(fractionalDecStr, 10);  

  // A magic number which is 1 more digit than the fractional part
  std::string magicStr(fractionalDecStr.length(), '0');
  magicStr = "1" + magicStr;
  int128::QInt magicFracqint(magicStr, 10);

  // Array of chars to store the fractional part in binary form
  char fractionalBinStr[FLOAT128_SIGNIF_LENGTH + 1];
  for (char& c : fractionalBinStr) {
    c = '0';
  }
  fractionalBinStr[FLOAT128_SIGNIF_LENGTH] = '\0';

  // Algorithm to convert the fraction from dec to bin
  for (size_t i = 0; i < FLOAT128_SIGNIF_LENGTH; ++i) {
    fractqint = fractqint << 1;

    if (fractqint == magicFracqint) {
      fractionalBinStr[i] = '1';
      break;
    }

    if (fractqint > magicFracqint) {
      fractionalBinStr[i] = '1';
      fractqint = fractqint - magicFracqint;
    }
  }

  return std::string(fractionalBinStr);
}

std::string QFloat::formatBinStr(const std::string& binStr) {
  std::string resultStr = binStr;
  int exp = 0;

  switch (binStr[0]) {
    case '1':
      // Find the exponent, move the floating point up
      // to normalize the bin string and erase the overflow bits
      exp = binStr.find('.') - 1;
      resultStr = resultStr.erase(binStr.find('.'), 1);
      resultStr = resultStr.insert(1, 1, '.');
      resultStr = resultStr.erase(FLOAT128_SIGNIF_LENGTH + 2);
      break;
    case '0':
      // Find the exponent, move the floating point down
      // to normalize the bin string and assign the trailing zeros
      exp = 1 - binStr.find_first_of('1');
      resultStr = resultStr.erase(binStr.find('.'), 1);
      resultStr =
          resultStr.insert(resultStr.find_first_not_of('0') + 1, 1, '.');
      resultStr = resultStr.substr(resultStr.find_first_not_of('0'));
      resultStr += std::string(abs(exp), '0');
      break;
  }

  // Format the result string to the form: 1.f * 2^e
  resultStr += "e" + std::to_string(exp);

  return resultStr;
}

std::string QFloat::decToBin(const std::string& decStr) {
  // Extract each part to each string
  std::string exponentDecStr = getExponentStr(decStr);
  std::string integralDecStr = getIntegralStr(decStr);
  std::string fractionalDecStr = getFractionalStr(decStr);

  // Convert integral and fractional part from dec to bin
  std::string integralBinStr = integralDecToBin(integralDecStr);
  std::string fractionalBinStr = fractionalDecToBin(fractionalDecStr);

  std::string signBinStr;
  std::string exponentBinStr;
  std::string significandStr;
  std::string normalizedBinStr;
  std::string binStr;

  // Help check for zero case
  bool integralIsOff =
      (integralBinStr.find('1') == std::string::npos) ? true : false;
  bool fractionalIsOff =
      (fractionalBinStr.find('1') == std::string::npos) ? true : false;

  // Zero case
  if (integralIsOff && fractionalIsOff) {
    signBinStr = "0";
    exponentBinStr = std::string(15, '0');
    significandStr = std::string(112, '0');

  // Other case
  } else {
    // Build and format the bin string
    normalizedBinStr = integralBinStr + "." + fractionalBinStr;
    normalizedBinStr = formatBinStr(normalizedBinStr);

    // Convert the exponent to the right format
    int exponent =
        std::stoi(normalizedBinStr.substr(normalizedBinStr.find('e') + 1)) +
        (int)(ceil(log2(10) * std::stoi(exponentDecStr))) + FLOAT128_BIAS;

    // Convert the exponent to special case when it is overflow
    if (exponent > FLOAT128_MAX_EXP + FLOAT128_BIAS) {
      exponentDecStr = std::to_string(FLOAT128_MAX_EXP + FLOAT128_BIAS);

    // Normal case
    } else {
      exponentDecStr = std::to_string(exponent);
    }

    // Extract each part and convert to bin
    signBinStr = (decStr[0] == '-') ? "1" : "0";
    exponentBinStr = exponentDecToBin(exponentDecStr);
    significandStr = normalizedBinStr.substr(normalizedBinStr.find('.') + 1,
                                             normalizedBinStr.find('e') - 2);
  }

  // Build the formatted bin string
  binStr = signBinStr + exponentBinStr + significandStr;

  return binStr;
}

std::string QFloat::binToDec(const std::string& binStr) {
  // Extract bits from bin str
  std::string signBinStr = binStr.substr(0, 1);
  std::string exponentBinStr = binStr.substr(1, 15);
  std::string significandStr = binStr.substr(16);

  // In the form of: 1.F
  std::string integralBinStr = "1";
  std::string fractionalBinStr = significandStr;
  std::string normalizedBinStr = integralBinStr + "." + fractionalBinStr;

  // Result str
  std::string decStr;

  // Exponent = 0
  if (exponentBinStr.find('1') == std::string::npos) {
    // Significand = 0 (zero number)
    if (significandStr.find('1') == std::string::npos) {
      decStr = "0";

      // Significand <> 0 (denormalized number)
    } else {
      // NOT IMPLEMENTED
    }

    // Exponent = all 1s
  } else if (exponentBinStr.find('0') == std::string::npos) {
    // Sinigficand = 0 => infinity; significand <> 0 => not a number
    decStr = (significandStr.find('1') == std::string::npos) ? decStr = "inf"
                                                             : decStr = "NaN";
  } else {
    // Assign the sign according to the first bit
    std::string signStr = (signBinStr == "1") ? "-" : "";

    // In the form of: A.B (A and B are in decimal base)
    std::string integralDecStr;
    std::string fractionalDecStr;

    // Calculate the real exponent
    std::string exponentDecStr =
        std::to_string(std::bitset<15>(exponentBinStr).to_ulong());
    int exponent = std::stoi(exponentDecStr) - FLOAT128_BIAS;

    // Assign leading zeros to the integral part
    // in case the floating point moves up
    integralBinStr = (exponent < 0)
                         ? std::string(abs(exponent), '0') + integralBinStr
                         : integralBinStr;

    // Rebuild the normalized bin str
    normalizedBinStr = integralBinStr + "." + fractionalBinStr;

    // Find the floating point position
    int dotPos = normalizedBinStr.find('.');

    if ((exponent > 0 && exponent <= FLOAT128_SIGNIF_LENGTH) ||
        (exponent < 0 && exponent >= -FLOAT128_SIGNIF_LENGTH)) {
      // Move the floating point to new position
      normalizedBinStr = normalizedBinStr.erase(dotPos, 1);
      normalizedBinStr = normalizedBinStr.insert(dotPos + exponent, 1, '.');

      // Extract the integral bin part and the fractional bin part
      // after moving the floating point
      dotPos = normalizedBinStr.find('.');
      integralBinStr = normalizedBinStr.substr(0, dotPos);
      fractionalBinStr = normalizedBinStr.substr(dotPos + 1);

      // Set exponent to 0
      exponent = (int)(ceil(log10(2) * ((long long)exponent - dotPos)));
    }

    // *** Convert the fractional part from bin to dec
    int128::QInt magicqint("10000000000000000000000000000000000", 10);
    int128::QInt magicSum = magicqint;
    int128::QInt qint2("2", 10);

    for (size_t i = 0; i < fractionalBinStr.length(); ++i) {
      if (fractionalBinStr[i] == '1') {
        magicSum = magicSum + magicqint / qint2.exp(i + 1);
      }
    }

    fractionalDecStr = magicSum.getDec().erase(0, 1);
    fractionalDecStr =
        fractionalDecStr.erase(fractionalDecStr.find_last_not_of('0') + 1);
    fractionalDecStr += (fractionalDecStr == "") ? "0" : "";
    // ***

    // Convert the integral part from bin to dec
    integralDecStr = int128::QInt::binToDec(integralBinStr);

    // Convert the exponent from base 2 to base 10
    exponent = (exponent == 0) ? 0 : (int)(ceil(log10(2) * exponent));

    // Build the complete dec string
    decStr = signStr + integralDecStr + "." + fractionalDecStr;
  }

  return decStr;
}
}  // namespace float128
}  // namespace native
