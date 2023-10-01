#pragma once

#include <functional>

namespace ac::registry {

template<typename Type, typename Tag>
class Tagged {
public:
  Tagged(Type& val)
   : _value(val)
  {}

  typedef Type type;
  typedef Tag tag;

  Type& Value() { return _value; }

private:
  Type &_value;
};

template<typename Type, typename Tag>
class Tagged<std::vector<std::reference_wrapper<Type>>, Tag> {
public:
  Tagged(std::vector<std::reference_wrapper<Type>>& val)
   : _value(val)
  {}

  typedef std::vector<std::reference_wrapper<Type>> type;
  typedef Tag tag;

  std::vector<std::reference_wrapper<Type>>& Value() { return _value; }

private:
  std::vector<std::reference_wrapper<Type>> _value;
};

template<typename T>
struct IsTaggedType : public std::false_type {};

template<typename Type, typename Tag>
struct IsTaggedType<Tagged<Type, Tag>> : public std::true_type {};

template<typename T>
concept TaggedType = IsTaggedType<T>::value;

} // namespace ac::registry
