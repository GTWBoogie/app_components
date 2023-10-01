#include "lockless_lifo_queue.h"

#define BOOST_TEST_MODULE LocklessLIFOQueue tests
#include <boost/test/included/unit_test.hpp>

#include <algorithm>

BOOST_AUTO_TEST_CASE(sequential_input_will_be_reversed)
{
  ac::util::LocklessLIFOQueue<int> queue;

  std::vector<int> input(10);

  std::generate(input.begin(), input.end(), [i = 0]() mutable { return i++; });

  for (int i : input)
    queue.push(i);

  std::vector<int> result;
  queue.consume_all([&result](const int i) { result.push_back(i); });

  std::reverse(result.begin(), result.end());

  BOOST_TEST(input == result);
}

BOOST_AUTO_TEST_CASE(sequential_input_will_be_reversed_by_parts)
{
  ac::util::LocklessLIFOQueue<int> queue;

  std::vector<int> result;

  queue.push(0);
  queue.push(1);

  queue.consume_all([&result](const int i) { result.push_back(i); });

  queue.push(2);

  queue.consume_all([&result](const int i) { result.push_back(i); });

  queue.push(3);
  queue.push(4);
  queue.push(5);

  queue.consume_all([&result](const int i) { result.push_back(i); });

  BOOST_TEST(result [0] == 1);
  BOOST_TEST(result [1] == 0);
  BOOST_TEST(result [2] == 2);
  BOOST_TEST(result [3] == 5);
  BOOST_TEST(result [4] == 4);
  BOOST_TEST(result [5] == 3);
}
