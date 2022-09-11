#include "tuple.h"

#define BOOST_TEST_MODULE tuple tests
#include <boost/test/included/unit_test.hpp>

#include <type_traits>

BOOST_AUTO_TEST_CASE(remove_first_tuple_element_test)
{
  using tuple_type = std::tuple<int, char, bool>;
  using desired_type = std::tuple<char, bool>;
  using result_type = util::remove_first_tuple_elements<1, tuple_type>::type;

  BOOST_TEST(std::tuple_size<result_type>::value == std::tuple_size<tuple_type>::value - 1);
  BOOST_TEST((std::is_same<desired_type, result_type>::value));
}

BOOST_AUTO_TEST_CASE(remove_first_two_tuple_elements_test)
{
  using tuple_type = std::tuple<char, int, bool>;
  using desired_type = std::tuple<bool>;
  using result_type = util::remove_first_tuple_elements<2, tuple_type>::type;

  BOOST_TEST(std::tuple_size<result_type>::value == std::tuple_size<tuple_type>::value - 2);
  BOOST_TEST((std::is_same<desired_type, result_type>::value));
}

BOOST_AUTO_TEST_CASE(remove_first_three_tuple_elements_test)
{
  using tuple_type = std::tuple<char, int, bool>;
  using desired_type = std::tuple<>;
  using result_type = util::remove_first_tuple_elements<3, tuple_type>::type;

  BOOST_TEST(std::tuple_size<result_type>::value == std::tuple_size<tuple_type>::value - 3);
  BOOST_TEST((std::is_same<desired_type, result_type>::value));
}

BOOST_AUTO_TEST_CASE(remove_more_elements_than_tuple_size_test)
{
  using tuple_type = std::tuple<char, int, bool>;
  using desired_type = std::tuple<>;
  using result_type = util::remove_first_tuple_elements<6, tuple_type>::type;

  BOOST_TEST(std::tuple_size<result_type>::value == 0);
  BOOST_TEST((std::is_same<desired_type, result_type>::value));
}
