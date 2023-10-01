#pragma once

#include <string>
#include <typeinfo>

namespace ac::util {

std::string get_demangled_type_name(const char *type_name);

std::string get_demangled_type_name(const std::type_info& type_info);

template<typename T>
std::string get_demangled_type_name()
{
  return get_demangled_type_name(typeid(T));
}

} // namespace ac::util
