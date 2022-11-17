#include "lockless_lifo_queue.h"

#define BOOST_TEST_MODULE LocklessLIFOQueue tests
#include <boost/test/included/unit_test.hpp>

#include <algorithm>

BOOST_AUTO_TEST_CASE(demangle_simple_types)
{
  util::LocklessLIFOQueue<int> queue;

  std::vector<int> input(10);

  std::generate(input.begin(), input.end(), [i = 0]() mutable { return i++; });

  for (int i : input)
    queue.push(i);

  std::vector<int> result;
  queue.consume_all([&result](const int i) { result.push_back(i); });

  std::reverse(result.begin(), result.end());

  BOOST_TEST(input == result);
}
