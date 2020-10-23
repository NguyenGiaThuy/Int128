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

bool strContainsAny(const string& str, const string any[]) {
  for (size_t i = 0; i < 15; ++i) {
    if (str.find(any[i]) != string::npos) {
      return true;
    }
  }
  return false;
}

string normalizedDecToBin(const string& dec) {
  string result = QInt::decToBin(dec);

  if (result.find('1') != string::npos) {
    result.erase(0, result.find_first_not_of('0'));
  } else {
    result.erase(result.begin() + 1, result.end());
  }

  return result;
}

string normalizedBinToDec(const string& bin) {
  string result = QInt::binToDec(bin);

  if (result[0] == '-') {
    result.erase(1, result.find_first_not_of('0', 1) - 1);
  } else {
    if (result.find_first_not_of('0') != string::npos) {
      result.erase(0, result.find_first_not_of('0'));
    } else {
      result.erase(result.begin() + 1, result.end());
    }
  }

  return result;
}

string normalizedHexToBin(const string& hex) {
  string result = QInt::hexToBin(hex);

  if (result.find('1') != string::npos) {
    result.erase(0, result.find_first_not_of('0'));
  } else {
    result.erase(result.begin() + 1, result.end());
  }

  return result;
}

string normalizedBinToHex(const string& bin) {
  string result = QInt::binToHex(bin);

  if (result.find_first_not_of('0') != string::npos) {
    result.erase(0, result.find_first_not_of('0'));
  } else {
    result.erase(result.begin() + 1, result.end());
  }

  return result;
}

string normalizedBin(const string& bin) {
  string result = bin;

  if (result.find('1') != string::npos) {
    result.erase(0, result.find_first_not_of('0'));
  } else {
    result.erase(result.begin() + 1, result.end());
  }

  return result;
}

string getBinResult(const vector<string>& tokens) {
  string result;

  if (tokens[2] == "+") {
    QInt temp = QInt(tokens[1], 2) + QInt(tokens[3], 2);
    result = normalizedBin(temp.getContent());
  } else if (tokens[2] == "-") {
    QInt temp = QInt(tokens[1], 2) - QInt(tokens[3], 2);
    result = normalizedBin(temp.getContent());
  } else if (tokens[2] == "*") {
    QInt temp = QInt(tokens[1], 2) * QInt(tokens[3], 2);
    result = normalizedBin(temp.getContent());
  } else if (tokens[2] == "/") {
    QInt temp = QInt(tokens[1], 2) / QInt(tokens[3], 2);
    result = normalizedBin(temp.getContent());
  } else if (tokens[2] == "<") {
    bool temp = QInt(tokens[1], 2) < QInt(tokens[3], 2);
    result = temp;
  } else if (tokens[2] == ">") {
    bool temp = QInt(tokens[1], 2) > QInt(tokens[3], 2);
    result = temp;
  } else if (tokens[2] == "<=") {
    bool temp = QInt(tokens[1], 2) <= QInt(tokens[3], 2);
    result = temp;
  } else if (tokens[2] == ">=") {
    bool temp = QInt(tokens[1], 2) >= QInt(tokens[3], 2);
    result = temp;
  } else if (tokens[2] == "==") {
    bool temp = QInt(tokens[1], 2) == QInt(tokens[3], 2);
    result = temp;
  } else if (tokens[2] == "=") {  // ??????????
  } else if (tokens[2] == "&") {
    QInt temp = QInt(tokens[1], 2) & QInt(tokens[3], 2);
    result = normalizedBin(temp.getContent());
  } else if (tokens[2] == "|") {
    QInt temp = QInt(tokens[1], 2) | QInt(tokens[3], 2);
    result = normalizedBin(temp.getContent());
  } else if (tokens[2] == "^") {
    QInt temp = QInt(tokens[1], 2) ^ QInt(tokens[3], 2);
    result = normalizedBin(temp.getContent());
  } else if (tokens[2] == "<<") {
    QInt temp = QInt(tokens[1], 2) << stoi(tokens[3]);
    result = normalizedBin(temp.getContent());
  } else if (tokens[2] == ">>") {
    QInt temp = QInt(tokens[1], 2) >> stoi(tokens[3]);
    result = normalizedBin(temp.getContent());
  }

  return result;
}

string getDecResult(const vector<string>& tokens) {
  string result;

  if (tokens[2] == "+") {
    QInt temp = QInt(QInt::decToBin(tokens[1]), 2) +
                QInt(QInt::decToBin(tokens[3]), 2);
    result = normalizedBinToDec(temp.getContent());
  } else if (tokens[2] == "-") {
    QInt temp = QInt(QInt::decToBin(tokens[1]), 2) -
                QInt(QInt::decToBin(tokens[3]), 2);
    result = normalizedBinToDec(temp.getContent());
  } else if (tokens[2] == "*") {
    QInt temp = QInt(QInt::decToBin(tokens[1]), 2) *
                QInt(QInt::decToBin(tokens[3]), 2);
    result = normalizedBinToDec(temp.getContent());
  } else if (tokens[2] == "/") {
    QInt temp = QInt(QInt::decToBin(tokens[1]), 2) /
                QInt(QInt::decToBin(tokens[3]), 2);
    result = normalizedBinToDec(temp.getContent());
  } else if (tokens[2] == "<") {
    bool temp = QInt(QInt::decToBin(tokens[1]), 2) <
                QInt(QInt::decToBin(tokens[3]), 2);
    result = temp;
  } else if (tokens[2] == ">") {
    bool temp = QInt(QInt::decToBin(tokens[1]), 2) >
                QInt(QInt::decToBin(tokens[3]), 2);
    result = temp;
  } else if (tokens[2] == "<=") {
    bool temp = QInt(QInt::decToBin(tokens[1]), 2) <=
                QInt(QInt::decToBin(tokens[3]), 2);
    result = temp;
  } else if (tokens[2] == ">=") {
    bool temp = QInt(QInt::decToBin(tokens[1]), 2) >=
                QInt(QInt::decToBin(tokens[3]), 2);
    result = temp;
  } else if (tokens[2] == "==") {
    bool temp = QInt(QInt::decToBin(tokens[1]), 2) ==
                QInt(QInt::decToBin(tokens[3]), 2);
    result = temp;
  } else if (tokens[2] == "=") {  // ??????????
  } else if (tokens[2] == "&") {
    QInt temp = QInt(QInt::decToBin(tokens[1]), 2) &
                QInt(QInt::decToBin(tokens[3]), 2);
    result = normalizedBinToDec(temp.getContent());
  } else if (tokens[2] == "|") {
    QInt temp = QInt(QInt::decToBin(tokens[1]), 2) |
                QInt(QInt::decToBin(tokens[3]), 2);
    result = normalizedBinToDec(temp.getContent());
  } else if (tokens[2] == "^") {
    QInt temp = QInt(QInt::decToBin(tokens[1]), 2) ^
                QInt(QInt::decToBin(tokens[3]), 2);
    result = normalizedBinToDec(temp.getContent());
  } else if (tokens[2] == "<<") {
    QInt temp = QInt(QInt::decToBin(tokens[1]), 2) << stoi(tokens[3]);
    result = normalizedBinToDec(temp.getContent());
  } else if (tokens[2] == ">>") {
    QInt temp = QInt(QInt::decToBin(tokens[1]), 2) >> stoi(tokens[3]);
    result = normalizedBinToDec(temp.getContent());
  }

  return result;
}

string getHexResult(const vector<string>& tokens) {
  string result;

  if (tokens[2] == "+") {
    QInt temp = QInt(QInt::hexToBin(tokens[1]), 2) +
                QInt(QInt::hexToBin(tokens[3]), 2);
    result = normalizedBinToHex(temp.getContent());
  } else if (tokens[2] == "-") {
    QInt temp = QInt(QInt::hexToBin(tokens[1]), 2) -
                QInt(QInt::hexToBin(tokens[3]), 2);
    result = normalizedBinToHex(temp.getContent());
  } else if (tokens[2] == "*") {
    QInt temp = QInt(QInt::hexToBin(tokens[1]), 2) *
                QInt(QInt::hexToBin(tokens[3]), 2);
    result = normalizedBinToHex(temp.getContent());
  } else if (tokens[2] == "/") {
    QInt temp = QInt(QInt::hexToBin(tokens[1]), 2) /
                QInt(QInt::hexToBin(tokens[3]), 2);
    result = normalizedBinToHex(temp.getContent());
  } else if (tokens[2] == "<") {
    bool temp = QInt(QInt::hexToBin(tokens[1]), 2) <
                QInt(QInt::hexToBin(tokens[3]), 2);
    result = temp;
  } else if (tokens[2] == ">") {
    bool temp = QInt(QInt::hexToBin(tokens[1]), 2) >
                QInt(QInt::hexToBin(tokens[3]), 2);
    result = temp;
  } else if (tokens[2] == "<=") {
    bool temp = QInt(QInt::hexToBin(tokens[1]), 2) <=
                QInt(QInt::hexToBin(tokens[3]), 2);
    result = temp;
  } else if (tokens[2] == ">=") {
    bool temp = QInt(QInt::hexToBin(tokens[1]), 2) >=
                QInt(QInt::hexToBin(tokens[3]), 2);
    result = temp;
  } else if (tokens[2] == "==") {
    bool temp = QInt(QInt::hexToBin(tokens[1]), 2) ==
                QInt(QInt::hexToBin(tokens[3]), 2);
    result = temp;
  } else if (tokens[2] == "=") {  // ??????????
  } else if (tokens[2] == "&") {
    QInt temp = QInt(QInt::hexToBin(tokens[1]), 2) &
                QInt(QInt::hexToBin(tokens[3]), 2);
    result = normalizedBinToHex(temp.getContent());
  } else if (tokens[2] == "|") {
    QInt temp = QInt(QInt::hexToBin(tokens[1]), 2) |
                QInt(QInt::hexToBin(tokens[3]), 2);
    result = normalizedBinToHex(temp.getContent());
  } else if (tokens[2] == "^") {
    QInt temp = QInt(QInt::hexToBin(tokens[1]), 2) ^
                QInt(QInt::hexToBin(tokens[3]), 2);
    result = normalizedBinToHex(temp.getContent());
  } else if (tokens[2] == "<<") {
    QInt temp = QInt(QInt::hexToBin(tokens[1]), 2) << stoi(tokens[3]);
    result = normalizedBinToHex(temp.getContent());
  } else if (tokens[2] == ">>") {
    QInt temp = QInt(QInt::hexToBin(tokens[1]), 2) >> stoi(tokens[3]);
    result = normalizedBinToHex(temp.getContent());
  }

  return result;
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
      string result;

      if (strContainsAny(str, binaryOperators)) {
        switch (stoi(tokens[0])) {
          case 2:
            result = getBinResult(tokens);
            break;
          case 10:
            result = getDecResult(tokens);
            break;
          case 16:
            result = getHexResult(tokens);
            break;
        }
      } else if (strContainsAny(str, unaryOperators)) {  // NEED IMPLEMENTATION
      } else {
        string temp;

        switch (stoi(tokens[0])) {
          case 2:
            temp = QInt(tokens[2], 2).getContent();
            break;
          case 10:
            temp = QInt(tokens[2], 10).getContent();
            break;
          case 16:
            temp = QInt(tokens[2], 16).getContent();
            break;
        }

        switch (stoi(tokens[1])) {
          case 2:
            result = normalizedBin(temp);
            break;
          case 10:
            result = normalizedBinToDec(temp);
            break;
          case 16:
            result = normalizedBinToHex(temp);
            break;
        }
      }

      writer << result << endl;
    }

    reader.close();
    writer.close();
  }
}

int main(int argc, char* argv[]) {
  //processQInt("input.txt", "output.txt");
  QInt a("1", 10);
  QInt b("1", 10);
  QInt c;
  c = a + b;
  cout << c.getContent();

  return 0;
}