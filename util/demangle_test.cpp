#include "demangle.h"

#define BOOST_TEST_MODULE demangle tests
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(demangle_simple_types)
{
  BOOST_TEST(ac::util::get_demangled_type_name<bool>() == "bool");
  BOOST_TEST(ac::util::get_demangled_type_name<char>() == "char");
  BOOST_TEST(ac::util::get_demangled_type_name<unsigned char>() == "unsigned char");
  BOOST_TEST(ac::util::get_demangled_type_name<int>() == "int");
  BOOST_TEST(ac::util::get_demangled_type_name<unsigned int>() == "unsigned int");
  BOOST_TEST(ac::util::get_demangled_type_name<long long>() == "long long");
  BOOST_TEST(ac::util::get_demangled_type_name<unsigned long long>() == "unsigned long long");
  BOOST_TEST(ac::util::get_demangled_type_name<float>() == "float");
  BOOST_TEST(ac::util::get_demangled_type_name<double>() == "double");
}

struct Foo {};
struct Bar {};

BOOST_AUTO_TEST_CASE(demangle_user_types)
{
  BOOST_TEST(ac::util::get_demangled_type_name<Foo>() == "Foo");
  BOOST_TEST(ac::util::get_demangled_type_name<Bar>() == "Bar");
}

namespace baz {
  struct Foo {};
  struct Bar {};
} // namespace baz

BOOST_AUTO_TEST_CASE(demangle_user_types_from_namespace)
{
  BOOST_TEST(ac::util::get_demangled_type_name<baz::Foo>() == "baz::Foo");
  BOOST_TEST(ac::util::get_demangled_type_name<baz::Bar>() == "baz::Bar");
}

template<typename T>
struct Fizz {};

BOOST_AUTO_TEST_CASE(demangle_template_types)
{
  BOOST_TEST(ac::util::get_demangled_type_name<Fizz<baz::Foo>>() == "Fizz<baz::Foo>");
  BOOST_TEST(ac::util::get_demangled_type_name<Fizz<double>>() == "Fizz<double>");
}
