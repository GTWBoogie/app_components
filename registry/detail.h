#pragma once

#include "dependencies.h"
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

template<typename Type>
struct ComponentInstanceGetter<ComponentContainer<Type>> {
  static std::vector<Instance> Get(ProviderBase& cp)
  {
    return cp.GetComponentInstances(std::type_index(typeid(Type)));
  }
};

template<typename Type, typename Tag>
struct ComponentInstanceGetter<Tagged<ComponentContainer<Type>, Tag>> {
  static std::vector<Instance> Get(ProviderBase& cp)
  {
    return cp.GetComponentInstances(std::type_index(typeid(Type)),
                                    std::type_index(typeid(Tag)));
  }
};

template<typename T>
struct Referencer {
  static std::tuple<T&> Get(Instance& instance)
  {
    return std::tuple<T&>(*std::any_cast<T*>(*instance.instance));
  }

  static std::tuple<T> Get(std::vector<Instance>& instances)
  {
    T result;
    for (auto &ci: instances) {
      result.push_back(*std::any_cast<typename T::value_type::type *>(*ci.instance.get()));
    }
    return std::tuple<T>(result);
  }
};

template<TaggedType T>
struct Referencer<T> {
  static std::tuple<T> Get(Instance& instance)
  {
    T wrapped(*std::any_cast<typename T::type*>(*instance.instance));
    return std::tuple<T&>(wrapped);
  }

  static std::tuple<T> Get(std::vector<Instance>& instances)
  {
    typename T::type result;
    for (auto &ci: instances) {
      result.push_back(*std::any_cast<typename T::type::value_type::type *>(*ci.instance.get()));
    }
    return std::tuple<T>(T(result));
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
    AddDependencies(dependencies, instance);
    return Referencer<type>::Get(instance);
  }
  else
  {
    typedef typename std::remove_reference<typename std::tuple_element<0, T>::type>::type type;
    auto instance = ComponentInstanceGetter<type>::Get(cp);
    AddDependencies(dependencies, instance);
    return std::tuple_cat(Referencer<type>::Get(instance),
            MakeArgumentsTuple<typename util::remove_first_tuple_elements<1, T>::type>(cp, dependencies));
  }
}

} // namespace detail
