#include "stop_token.h"

#define BOOST_TEST_MODULE stop_token tests
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(single_stop_token)
{
  util::stop_source source;

  auto token = source.get_token();

  BOOST_TEST(source.stop_possible() == true);
  BOOST_TEST(token.stop_possible() == true);
  BOOST_TEST(token.stop_requested() == false);
  BOOST_TEST(source.request_stop() == true);
  BOOST_TEST(source.stop_possible() == false);
  BOOST_TEST(token.stop_possible() == false);
  BOOST_TEST(token.stop_requested() == true);
  BOOST_TEST(source.request_stop() == false);
  BOOST_TEST(token.stop_requested() == true);
}

BOOST_AUTO_TEST_CASE(two_stop_token_instances_from_single_source)
{
  util::stop_source source;

  auto token1 = source.get_token();
  auto token2 = source.get_token();

  BOOST_TEST(source.stop_possible() == true);
  BOOST_TEST(token1.stop_possible() == true);
  BOOST_TEST(token2.stop_possible() == true);
  BOOST_TEST(token1.stop_requested() == false);
  BOOST_TEST(token2.stop_requested() == false);
  BOOST_TEST(source.request_stop() == true);
  BOOST_TEST(source.stop_possible() == false);
  BOOST_TEST(token1.stop_possible() == false);
  BOOST_TEST(token2.stop_possible() == false);
  BOOST_TEST(token1.stop_requested() == true);
  BOOST_TEST(token2.stop_requested() == true);
  BOOST_TEST(source.request_stop() == false);
  BOOST_TEST(token1.stop_requested() == true);
  BOOST_TEST(token2.stop_requested() == true);
}

BOOST_AUTO_TEST_CASE(stop_token_instance_copied_from_previous)
{
  util::stop_source source;

  auto token1 = source.get_token();
  auto token2(token1);

  BOOST_TEST(source.stop_possible() == true);
  BOOST_TEST(token1.stop_possible() == true);
  BOOST_TEST(token2.stop_possible() == true);
  BOOST_TEST(token1.stop_requested() == false);
  BOOST_TEST(token2.stop_requested() == false);
  BOOST_TEST(source.request_stop() == true);
  BOOST_TEST(source.stop_possible() == false);
  BOOST_TEST(token1.stop_possible() == false);
  BOOST_TEST(token2.stop_possible() == false);
  BOOST_TEST(token1.stop_requested() == true);
  BOOST_TEST(token2.stop_requested() == true);
  BOOST_TEST(source.request_stop() == false);
  BOOST_TEST(token1.stop_requested() == true);
  BOOST_TEST(token2.stop_requested() == true);
}
