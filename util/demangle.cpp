#include "demangle.h"

#include <boost/core/demangle.hpp>

namespace util {

std::string get_demangled_type_name(const char *type_name)
{
  return boost::core::demangle(type_name);
}

std::string get_demangled_type_name(const std::type_info& type_info)
{
  return get_demangled_type_name(type_info.name());
}

} // namespace util
