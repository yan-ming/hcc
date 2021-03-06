// XFAIL: Linux
// RUN: %hc %s -o %t.out && %t.out

// Parallel STL headers
#include <coordinate>
#include <experimental/algorithm>
#include <experimental/execution_policy>

#define _DEBUG (0)
#include "test_base.h"


template<typename T, size_t SIZE>
bool test(void) {

  using std::experimental::parallel::par;

  bool ret = true;
  bool eq = true;
  // C array
  typedef T cArray[SIZE];
  ret &= run_and_compare<T, SIZE>([&eq](cArray &input, cArray &output1,
                                                       cArray &output2) {
    // test adjacent_find (non-predicated version)
    auto expected = std::adjacent_find(std::begin(input), std::end(input));
    auto result   = std::experimental::parallel::
                    adjacent_find(par, std::begin(input), std::end(input));

    eq = EQ(expected, result);
  }, false);
  ret &= eq;

  // C array
  ret &= run_and_compare<T, SIZE>([&eq](cArray &input, cArray &output1,
                                                       cArray &output2) {
    // make them equal
    input[2] = input[3];
    auto expected = std::adjacent_find(std::begin(input), std::end(input));
    auto result   = std::experimental::parallel::
                    adjacent_find(par, std::begin(input), std::end(input));

    eq = EQ(expected, result);
  }, false);
  ret &= eq;

  // use custom predicate
  auto pred = [](const T& a, const T& b) { return a > b; };

  ret &= run_and_compare<T, SIZE>([&eq, pred](cArray &input, cArray &output1,
                                                             cArray &output2) {
    // test adjacent_find (predicated version)
    auto expected = std::adjacent_find(std::begin(input), std::end(input), pred);
    auto result   = std::experimental::parallel::
                    adjacent_find(par, std::begin(input), std::end(input), pred);

    eq = EQ(expected, result);
  }, false);
  ret &= eq;

  return ret;
}

int main() {
  bool ret = true;

  ret &= test<int, TEST_SIZE>();
  ret &= test<unsigned, TEST_SIZE>();
  ret &= test<float, TEST_SIZE>();
  ret &= test<double, TEST_SIZE>();


  return !(ret == true);
}

