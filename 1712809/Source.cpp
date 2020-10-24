#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include "../Int128/Int128.h"

using namespace std;
using namespace Native::Int128;

vector<string> strSplit(const string& str) {
  istringstream iss(str);
  vector<string> tokens;
  copy(istream_iterator<string>(iss), istream_iterator<string>(),
       back_inserter(tokens));
  return tokens;
}

bool strContainsAny(const string& str, const string anyStr[]) {
  for (size_t i = 0; i < 15; ++i) {
    if (str.find(anyStr[i]) != string::npos) {
      return true;
    }
  }
  return false;
}

string normalizeDecToBin(const string& decStr) {
  string resultStr = QInt::decToBin(validateDec(decStr));

  if (resultStr.find('1') != string::npos) {
    resultStr.erase(0, resultStr.find_first_not_of('0'));
  } else {
    resultStr.erase(resultStr.begin() + 1, resultStr.end());
  }

  return resultStr;
}

string normalizeBinToDec(const string& binStr) {
  string resultStr = QInt::binToDec(binStr);

  if (resultStr[0] == '-') {
    resultStr.erase(1, resultStr.find_first_not_of('0', 1) - 1);
  } else {
    if (resultStr.find_first_not_of('0') != string::npos) {
      resultStr.erase(0, resultStr.find_first_not_of('0'));
    } else {
      resultStr.erase(resultStr.begin() + 1, resultStr.end());
    }
  }

  return resultStr;
}

string normalizeHexToBin(const string& hexStr) {
  string resultStr = QInt::hexToBin(validateHex(hexStr));

  if (resultStr.find('1') != string::npos) {
    resultStr.erase(0, resultStr.find_first_not_of('0'));
  } else {
    resultStr.erase(resultStr.begin() + 1, resultStr.end());
  }

  return resultStr;
}

string normalizeBinToHex(const string& binStr) {
  string resultStr = QInt::binToHex(binStr);

  if (resultStr.find_first_not_of('0') != string::npos) {
    resultStr.erase(0, resultStr.find_first_not_of('0'));
  } else {
    resultStr.erase(resultStr.begin() + 1, resultStr.end());
  }

  return resultStr;
}

string normalizeBin(const string& binStr) {
  string resultStr = binStr;

  if (resultStr.find('1') != string::npos) {
    resultStr.erase(0, resultStr.find_first_not_of('0'));
  } else {
    resultStr.erase(resultStr.begin() + 1, resultStr.end());
  }

  return resultStr;
}

string getBinResult(const vector<string>& tokens) {
  string resultStr;

  if (tokens[2] == "+") {
    QInt result = QInt(tokens[1], 2) + QInt(tokens[3], 2);
    resultStr = normalizeBin(result.getContent());
  } else if (tokens[2] == "-") {
    QInt result = QInt(tokens[1], 2) - QInt(tokens[3], 2);
    resultStr = normalizeBin(result.getContent());
  } else if (tokens[2] == "*") {
    QInt result = QInt(tokens[1], 2) * QInt(tokens[3], 2);
    resultStr = normalizeBin(result.getContent());
  } else if (tokens[2] == "/") {
    QInt result = QInt(tokens[1], 2) / QInt(tokens[3], 2);
    resultStr = normalizeBin(result.getContent());
  } else if (tokens[2] == "<") {
    bool result = QInt(tokens[1], 2) < QInt(tokens[3], 2);
    resultStr = result;
  } else if (tokens[2] == ">") {
    bool result = QInt(tokens[1], 2) > QInt(tokens[3], 2);
    resultStr = result;
  } else if (tokens[2] == "<=") {
    bool result = QInt(tokens[1], 2) <= QInt(tokens[3], 2);
    resultStr = result;
  } else if (tokens[2] == ">=") {
    bool result = QInt(tokens[1], 2) >= QInt(tokens[3], 2);
    resultStr = result;
  } else if (tokens[2] == "==") {
    bool result = QInt(tokens[1], 2) == QInt(tokens[3], 2);
    resultStr = result;
  } else if (tokens[2] == "=") {  // ??????????
  } else if (tokens[2] == "&") {
    QInt result = QInt(tokens[1], 2) & QInt(tokens[3], 2);
    resultStr = normalizeBin(result.getContent());
  } else if (tokens[2] == "|") {
    QInt result = QInt(tokens[1], 2) | QInt(tokens[3], 2);
    resultStr = normalizeBin(result.getContent());
  } else if (tokens[2] == "^") {
    QInt result = QInt(tokens[1], 2) ^ QInt(tokens[3], 2);
    resultStr = normalizeBin(result.getContent());
  } else if (tokens[2] == "<<") {
    QInt result = QInt(tokens[1], 2) << stoi(tokens[3]);
    resultStr = normalizeBin(result.getContent());
  } else if (tokens[2] == ">>") {
    QInt result = QInt(tokens[1], 2) >> stoi(tokens[3]);
    resultStr = normalizeBin(result.getContent());
  }

  return resultStr;
}

string getDecResult(const vector<string>& tokens) {
  string resultStr;

  if (tokens[2] == "+") {
    QInt result = QInt(QInt::decToBin(validateDec(tokens[1])), 2) +
                QInt(QInt::decToBin(validateDec(tokens[3])), 2);
    resultStr = normalizeBinToDec(result.getContent());
  } else if (tokens[2] == "-") {
    QInt result = QInt(QInt::decToBin(validateDec(tokens[1])), 2) -
                QInt(QInt::decToBin(validateDec(tokens[3])), 2);
    resultStr = normalizeBinToDec(result.getContent());
  } else if (tokens[2] == "*") {
    QInt result = QInt(QInt::decToBin(validateDec(tokens[1])), 2) *
                QInt(QInt::decToBin(validateDec(tokens[3])), 2);
    resultStr = normalizeBinToDec(result.getContent());
  } else if (tokens[2] == "/") {
    QInt result = QInt(QInt::decToBin(validateDec(tokens[1])), 2) /
                QInt(QInt::decToBin(validateDec(tokens[3])), 2);
    resultStr = normalizeBinToDec(result.getContent());
  } else if (tokens[2] == "<") {
    bool result = QInt(QInt::decToBin(validateDec(tokens[1])), 2) <
                QInt(QInt::decToBin(validateDec(tokens[3])), 2);
    resultStr = result;
  } else if (tokens[2] == ">") {
    bool result = QInt(QInt::decToBin(validateDec(tokens[1])), 2) >
                QInt(QInt::decToBin(validateDec(tokens[3])), 2);
    resultStr = result;
  } else if (tokens[2] == "<=") {
    bool result = QInt(QInt::decToBin(validateDec(tokens[1])), 2) <=
                QInt(QInt::decToBin(validateDec(tokens[3])), 2);
    resultStr = result;
  } else if (tokens[2] == ">=") {
    bool result = QInt(QInt::decToBin(validateDec(tokens[1])), 2) >=
                QInt(QInt::decToBin(validateDec(tokens[3])), 2);
    resultStr = result;
  } else if (tokens[2] == "==") {
    bool result = QInt(QInt::decToBin(validateDec(tokens[1])), 2) ==
                QInt(QInt::decToBin(validateDec(tokens[3])), 2);
    resultStr = result;
  } else if (tokens[2] == "=") {  // ??????????
  } else if (tokens[2] == "&") {
    QInt result = QInt(QInt::decToBin(validateDec(tokens[1])), 2) &
                QInt(QInt::decToBin(validateDec(tokens[3])), 2);
    resultStr = normalizeBinToDec(result.getContent());
  } else if (tokens[2] == "|") {
    QInt result = QInt(QInt::decToBin(validateDec(tokens[1])), 2) |
                QInt(QInt::decToBin(validateDec(tokens[3])), 2);
    resultStr = normalizeBinToDec(result.getContent());
  } else if (tokens[2] == "^") {
    QInt result = QInt(QInt::decToBin(validateDec(tokens[1])), 2) ^
                QInt(QInt::decToBin(validateDec(tokens[3])), 2);
    resultStr = normalizeBinToDec(result.getContent());
  } else if (tokens[2] == "<<") {
    QInt result = QInt(QInt::decToBin(validateDec(tokens[1])), 2) << stoi(tokens[3]);
    resultStr = normalizeBinToDec(result.getContent());
  } else if (tokens[2] == ">>") {
    QInt result = QInt(QInt::decToBin(validateDec(tokens[1])), 2) >> stoi(tokens[3]);
    resultStr = normalizeBinToDec(result.getContent());
  }

  return resultStr;
}

string getHexResult(const vector<string>& tokens) {
  string resultStr;

  if (tokens[2] == "+") {
    QInt result = QInt(QInt::hexToBin(validateHex(tokens[1])), 2) +
                QInt(QInt::hexToBin(validateHex(tokens[3])), 2);
    resultStr = normalizeBinToHex(result.getContent());
  } else if (tokens[2] == "-") {
    QInt result = QInt(QInt::hexToBin(validateHex(tokens[1])), 2) -
                QInt(QInt::hexToBin(validateHex(tokens[3])), 2);
    resultStr = normalizeBinToHex(result.getContent());
  } else if (tokens[2] == "*") {
    QInt result = QInt(QInt::hexToBin(validateHex(tokens[1])), 2) *
                QInt(QInt::hexToBin(validateHex(tokens[3])), 2);
    resultStr = normalizeBinToHex(result.getContent());
  } else if (tokens[2] == "/") {
    QInt result = QInt(QInt::hexToBin(validateHex(tokens[1])), 2) /
                QInt(QInt::hexToBin(validateHex(tokens[3])), 2);
    resultStr = normalizeBinToHex(result.getContent());
  } else if (tokens[2] == "<") {
    bool result = QInt(QInt::hexToBin(validateHex(tokens[1])), 2) <
                QInt(QInt::hexToBin(validateHex(tokens[3])), 2);
    resultStr = result;
  } else if (tokens[2] == ">") {
    bool result = QInt(QInt::hexToBin(validateHex(tokens[1])), 2) >
                QInt(QInt::hexToBin(validateHex(tokens[3])), 2);
    resultStr = result;
  } else if (tokens[2] == "<=") {
    bool result = QInt(QInt::hexToBin(validateHex(tokens[1])), 2) <=
                QInt(QInt::hexToBin(validateHex(tokens[3])), 2);
    resultStr = result;
  } else if (tokens[2] == ">=") {
    bool result = QInt(QInt::hexToBin(validateHex(tokens[1])), 2) >=
                QInt(QInt::hexToBin(validateHex(tokens[3])), 2);
    resultStr = result;
  } else if (tokens[2] == "==") {
    bool result = QInt(QInt::hexToBin(validateHex(tokens[1])), 2) ==
                QInt(QInt::hexToBin(validateHex(tokens[3])), 2);
    resultStr = result;
  } else if (tokens[2] == "=") {  // ??????????
  } else if (tokens[2] == "&") {
    QInt result = QInt(QInt::hexToBin(validateHex(tokens[1])), 2) &
                QInt(QInt::hexToBin(validateHex(tokens[3])), 2);
    resultStr = normalizeBinToHex(result.getContent());
  } else if (tokens[2] == "|") {
    QInt result = QInt(QInt::hexToBin(validateHex(tokens[1])), 2) |
                QInt(QInt::hexToBin(validateHex(tokens[3])), 2);
    resultStr = normalizeBinToHex(result.getContent());
  } else if (tokens[2] == "^") {
    QInt result = QInt(QInt::hexToBin(validateHex(tokens[1])), 2) ^
                QInt(QInt::hexToBin(validateHex(tokens[3])), 2);
    resultStr = normalizeBinToHex(result.getContent());
  } else if (tokens[2] == "<<") {
    QInt result = QInt(QInt::hexToBin(validateHex(tokens[1])), 2) << stoi(tokens[3]);
    resultStr = normalizeBinToHex(result.getContent());
  } else if (tokens[2] == ">>") {
    QInt result = QInt(QInt::hexToBin(validateHex(tokens[1])), 2) >> stoi(tokens[3]);
    resultStr = normalizeBinToHex(result.getContent());
  }

  return resultStr;
}

void processQInt(const char* inputFileName, const char* outputFileName) {
  string binaryOperators[] = {"+",  "-", "*", "/", "<", ">",  "<=", ">=",
                              "==", "=", "&", "|", "^", "<<", ">>"};
  string unaryOperators[] = {"~", "rol", "ror"};

  ifstream reader(inputFileName, ios::in);
  ofstream writer(outputFileName, ios::out);
  if (reader.is_open() && writer.is_open()) {
    string str;
    while (getline(reader, str)) {
      vector<string> tokens = strSplit(str);
      string resultStr;

      if (strContainsAny(str, binaryOperators)) {
        switch (stoi(tokens[0])) {
          case 2:
            resultStr = getBinResult(tokens);
            break;
          case 10:
            resultStr = getDecResult(tokens);
            break;
          case 16:
            resultStr = getHexResult(tokens);
            break;
        }
      } else if (strContainsAny(str, unaryOperators)) {  // NEED IMPLEMENTATION
      } else {
        string numberStr;

        switch (stoi(tokens[0])) {
          case 2:
            numberStr = QInt(tokens[2], 2).getContent();
            break;
          case 10:
            numberStr = QInt(tokens[2], 10).getContent();
            break;
          case 16:
            numberStr = QInt(tokens[2], 16).getContent();
            break;
        }

        switch (stoi(tokens[1])) {
          case 2:
            resultStr = normalizeBin(numberStr);
            break;
          case 10:
            resultStr = normalizeBinToDec(numberStr);
            break;
          case 16:
            resultStr = normalizeBinToHex(numberStr);
            break;
        }
      }

      writer << resultStr << endl;
    }

    reader.close();
    writer.close();
  }
}

int main(int argc, char* argv[]) {
  processQInt("input.txt", "output.txt");
  return 0;
}