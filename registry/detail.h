#pragma once

#include "converter.h"
#include "provider.h"
#include "creator.h"
#include "util/debug_log.h"
#include "util/demangle.h"
#include "util/tuple.h"

#include <boost/callable_traits.hpp>

#include <any>
#include <functional>
#include <typeindex>

namespace detail {

template<typename T>
concept Creatable = requires()
{
  T::Create;
  std::is_same<typename boost::callable_traits::return_type<decltype(&T::Create)>::type, T*>::value;
};

template<typename ObjectType>
auto GetDeleter()
{
  return [](std::any *ptr) {
    ObjectType *pointer = std::any_cast<ObjectType *>(*ptr);
    DLOG("Deleting object of type " << util::get_demangled_type_name<ObjectType>() << " [" << pointer << "]");
    delete pointer;
    delete ptr;
  };
}

template<typename From, typename To>
Converter GetConverter()
{
  if constexpr (std::is_same<From, To>::value)
  {
    return Converter([](const AnyPtr& ptr) { return ptr; });
  }
  else
  {
    return Converter([](const AnyPtr& ptr) {
      To *to = dynamic_cast<To*>(std::any_cast<From*>(*ptr.get()));
      if (to == nullptr)
      {
        throw std::invalid_argument(std::string("Object of type " + util::get_demangled_type_name<From>() +
                                                " does not provide interface ") + util::get_demangled_type_name<To>());
      }
      return AnyPtr(new std::any(to));
    });
  }
}

template<typename T>
T MakeArgumentsTuple(Provider& cp, std::vector<AnyPtr>& dependencies)
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
    auto instance = cp.GetComponentInstance(std::type_index(typeid(type)));
    dependencies.push_back(instance.instance);
    dependencies.insert(dependencies.end(), instance.dependencies.begin(), instance.dependencies.end());
    return T(*std::any_cast<type *>(*instance.instance));
  }
  else
  {
    typedef typename std::remove_reference<typename std::tuple_element<0, T>::type>::type type;
    auto instance = cp.GetComponentInstance(std::type_index(typeid(type)));
    dependencies.push_back(instance.instance);
    dependencies.insert(dependencies.end(), instance.dependencies.begin(), instance.dependencies.end());
    return std::tuple_cat(std::tuple<type&>(*std::any_cast<type *>(*instance.instance)),
            MakeArgumentsTuple<typename util::remove_first_tuple_elements<1, T>::type>(cp, dependencies));
  }
}

template<typename T>
CreatorPtr GetDefaultConstructibleCreator()
{
  return Creator::Create([](Provider&) {
    Instance ci;
    T* object = new T();
    DLOG("Created object of type " << util::get_demangled_type_name<T>() << " [" << object << "]");
    ci.instance = AnyPtr(new std::any(object), GetDeleter<T>());
    return ci;
  });
}

template<typename T>
CreatorPtr GetCreateConstructibleCreator()
{
  return Creator::Create([](Provider& provider) {
    Instance ci;
    using Arguments = typename boost::callable_traits::args<decltype(&T::Create)>::type;
    Arguments argument_tuple = MakeArgumentsTuple<Arguments>(provider, ci.dependencies);

    T* object = std::apply(T::Create, argument_tuple);
    DLOG("Created object of type " << util::get_demangled_type_name<T>() << " [" << object << "]");
    ci.instance = AnyPtr(new std::any(object), GetDeleter<T>());
    return ci;
  });
}

template<typename Callable>
CreatorPtr GetCreator(Callable f)
{
  typedef typename std::remove_pointer<typename boost::callable_traits::return_type<Callable>::type>::type return_type;

  return Creator::Create([f](Provider& provider) {
    Instance ci;
    using Arguments = typename boost::callable_traits::args<Callable>::type;
    Arguments argument_tuple = MakeArgumentsTuple<Arguments>(provider, ci.dependencies);

    return_type * object = std::apply(f, argument_tuple);
    DLOG("Created object of type " << util::get_demangled_type_name<return_type>() << " [" << object << "]");
    ci.instance = AnyPtr(new std::any(std::move(object)), GetDeleter<return_type>());
    return ci;
  });
}

template<typename T>
CreatorPtr GetAdaptedObjectCreator(T* object, bool managed = true)
{
  DLOG("Adapted object of type " << util::get_demangled_type_name<T>() << " [" << object << "]");

  AnyPtr object_pointer;
  if (managed)
  {
    object_pointer = AnyPtr(new std::any(std::move(object)), detail::GetDeleter<T>());
  }
  else
  {
    object_pointer = std::make_shared<std::any>(std::move(object));
  }

  return Creator::Create([object_pointer](Provider&) {
    Instance ci;
    ci.instance = object_pointer;
    return ci;
  });
}

} // namespace detail
