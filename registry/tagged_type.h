#pragma once

#include <functional>

template<typename Type, typename Tag>
class Tagged {
public:
  Tagged(Type& val)
          : _value(val)
  {}

  typedef Type type;
  typedef Tag tag;

  Type& Value() const { return _value; }

private:
  Type &_value;
};

template<typename T>
struct IsTaggedType : public std::false_type {};

template<typename Type, typename Tag>
struct IsTaggedType<Tagged<Type, Tag>> : public std::true_type {};

template<typename T>
concept TaggedType = IsTaggedType<T>::value;