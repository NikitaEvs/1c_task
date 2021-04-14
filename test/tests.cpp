#include <gtest/gtest.h>

#include "Field/core.hpp"


TEST(Unit, True) {
  GameField field;
  std::ifstream input("../../data/true.json");

  input >> field;

  ASSERT_EQ(field.IsWinning(), true);
}

TEST(Unit, False) {
  GameField field;
  std::ifstream input("../../data/false.json");

  input >> field;

  ASSERT_EQ(field.IsWinning(), false);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
