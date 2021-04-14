#include <iostream>
#include "Field/core.hpp"


int main(int argc, char **argv) {
  if (argc < 1) {
    std::cout << "Filename required" << std::endl;
  }

  GameField field;
  std::ifstream input(argv[1]);
  input >> field;

  bool isWinning = field.IsWinning();

  if (isWinning) {
    std::cout << "Yes" << std::endl;
  } else {
    std::cout << "No" << std::endl;
  }

  return 0;
}
