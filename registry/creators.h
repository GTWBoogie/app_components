#pragma once

#include "provider_base.h"
#include "creator.h"
#include "util/debug_log.h"
#include "util/demangle.h"

#include <boost/callable_traits.hpp>

#include <any>

namespace detail {

template<typename T> auto MakeArgumentsTuple(ProviderBase&, std::vector<AnyPtr>&);

template<typename T>
concept Creatable = requires()
{
  T::Create;
  std::is_same<typename boost::callable_traits::return_type<decltype(&T::Create)>::type, T*>::value;
};

template<typename ObjectType>
auto GetDeleter() {
  return [](std::any *ptr) {
    ObjectType *pointer = std::any_cast<ObjectType *>(*ptr);
    DLOG("Deleting object of type " << util::get_demangled_type_name<ObjectType>() << " [" << pointer << "]");
    delete pointer;
    delete ptr;
  };
}

template<typename T>
CreatorPtr GetDefaultConstructibleCreator() {
  return Creator::Create([](ProviderBase &) {
    Instance ci;
    T *object = new T();
    DLOG("Created object of type " << util::get_demangled_type_name<T>() << " [" << object << "]");
    ci.instance = AnyPtr(new std::any(object), GetDeleter<T>());
    return ci;
  });
}

template<typename T>
CreatorPtr GetCreateConstructibleCreator() {
  return Creator::Create([](ProviderBase &provider) {
    DLOG("Will Create object of type " << util::get_demangled_type_name<T>());
    Instance ci;
    using Arguments = typename boost::callable_traits::args<decltype(&T::Create)>::type;
    auto argument_tuple = MakeArgumentsTuple<Arguments>(provider, ci.dependencies);

    T *object = std::apply(T::Create, argument_tuple);
    DLOG("Created object of type " << util::get_demangled_type_name<T>() << " [" << object << "]");
    ci.instance = AnyPtr(new std::any(object), GetDeleter<T>());
    return ci;
  });
}

template<typename Callable>
CreatorPtr GetCreator(Callable f) {
  typedef typename std::remove_pointer<typename boost::callable_traits::return_type<Callable>::type>::type return_type;

  return Creator::Create([f](ProviderBase &provider) {
    Instance ci;
    using Arguments = typename boost::callable_traits::args<Callable>::type;
    auto argument_tuple = MakeArgumentsTuple<Arguments>(provider, ci.dependencies);

    return_type *object = std::apply(f, argument_tuple);
    DLOG("Created object of type " << util::get_demangled_type_name<return_type>() << " [" << object << "]");
    ci.instance = AnyPtr(new std::any(std::move(object)), GetDeleter<return_type>());
    return ci;
  });
}

template<typename T>
CreatorPtr GetAdaptedObjectCreator(T *object, bool managed = true) {
  DLOG("Adapted object of type " << util::get_demangled_type_name<T>() << " [" << object << "]");

  AnyPtr object_pointer;
  if (managed) {
    object_pointer = AnyPtr(new std::any(std::move(object)), detail::GetDeleter<T>());
  } else {
    object_pointer = std::make_shared<std::any>(std::move(object));
  }

  return Creator::Create([object_pointer](ProviderBase &) {
    Instance ci;
    ci.instance = object_pointer;
    return ci;
  });
}

} // namespace detail