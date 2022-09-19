#pragma once

#include "provider.h"
#include "util/demangle.h"
#include "util/tuple.h"

#include <any>
#include <typeindex>

namespace detail {

template<typename Type>
struct ComponentInstanceGetter {
  static Instance Get(ProviderBase& cp)
  {
    return cp.GetComponentInstance(std::type_index(typeid(Type)));
  }
};

template<TaggedType Type>
struct ComponentInstanceGetter<Type> {
  static Instance Get(ProviderBase& cp)
  {
    return cp.GetComponentInstance(std::type_index(typeid(typename Type::type)),
                                   std::type_index(typeid(typename Type::tag)));
  }
};

template<typename T>
struct Referencer {
  static std::tuple<T&> Get(AnyPtr& ptr)
  {
    return std::tuple<T&>(*std::any_cast<T*>(*ptr));
  }
};

template<TaggedType T>
struct Referencer<T> {
  static std::tuple<T> Get(AnyPtr& ptr)
  {
    T wrapped(*std::any_cast<typename T::type*>(*ptr));
    return std::tuple<T&>(wrapped);
  }
};

template<typename T>
auto MakeArgumentsTuple(ProviderBase& cp, std::vector<AnyPtr>& dependencies)
{
  // todo: refactor, check cyclic dependencies, accept pointer types, accept vector of types

  const size_t size = std::tuple_size<T>::value;
  if constexpr (size == 0)
  {
    return T();
  }
  else if constexpr (size == 1)
  {
    typedef typename std::remove_reference<typename std::tuple_element<0, T>::type>::type type;
    auto instance = ComponentInstanceGetter<type>::Get(cp);
    dependencies.push_back(instance.instance);
    dependencies.insert(dependencies.end(), instance.dependencies.begin(), instance.dependencies.end());
    return Referencer<type>::Get(instance.instance);
  }
  else
  {
    typedef typename std::remove_reference<typename std::tuple_element<0, T>::type>::type type;
    auto instance = ComponentInstanceGetter<type>::Get(cp);
    dependencies.push_back(instance.instance);
    dependencies.insert(dependencies.end(), instance.dependencies.begin(), instance.dependencies.end());
    return std::tuple_cat(Referencer<type>::Get(instance.instance),
            MakeArgumentsTuple<typename util::remove_first_tuple_elements<1, T>::type>(cp, dependencies));
  }
}

} // namespace detail
