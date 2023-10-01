#pragma once

#include "converter.h"
#include "tagged_type.h"

#include "util/demangle.h"

namespace ac::registry::detail {

template<typename From, typename To>
struct ConverterCreator {
  static Converter Create() {
    return Converter([](const AnyPtr &ptr) {
      To *to = dynamic_cast<To *>(std::any_cast<From *>(*ptr.get()));
      if (to == nullptr) {
        throw std::invalid_argument(std::string("Object of type " + util::get_demangled_type_name<From>() +
                                                " does not provide interface ") + util::get_demangled_type_name<To>());
      }
      return AnyPtr(new std::any(to));
    });
  }
};

template<typename From>
struct ConverterCreator<From, From> {
  static Converter Create() {
    return Converter([](const AnyPtr &ptr) { return ptr; });
  }
};

template<typename From, TaggedType To>
struct ConverterCreator<From, To> {
  static Converter Create() {
    return Converter([](const AnyPtr &ptr) {
      typename To::type *to = dynamic_cast<typename To::type *>(std::any_cast<From *>(*ptr.get()));
      if (to == nullptr) {
        throw std::invalid_argument(std::string("Object of type " + util::get_demangled_type_name<From>() +
                                                " does not provide interface ") +
                                    util::get_demangled_type_name<typename To::type>());
      }
      return AnyPtr(new std::any(to));
    });
  }
};

template<typename From, typename To>
Converter GetConverter() {
  return ConverterCreator<From, To>::Create();
}

} // namespace ac::registry::detail
