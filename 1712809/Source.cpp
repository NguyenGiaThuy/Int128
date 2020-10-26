#include "fileprocessing.h"
#include "testdrive.h"

using namespace fileprocessing;
using namespace testdrive;

int main(int argc, char* argv[]) {
  if (argc < 4) {
    cout << "Invalid arguments";
    return -1;
  }

  if (!strcmp(argv[3], "1")) {
    test(argv[1], argv[2]);
  } else {
  }

  return 0;
}