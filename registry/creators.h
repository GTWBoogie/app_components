#pragma once

#include "provider_base.h"
#include "creator.h"
#include "util/debug_log.h"
#include "util/demangle.h"

#include <boost/callable_traits.hpp>

#include <any>

#ifndef CONSTRUCTOR
  #define CONSTRUCTOR(signature) using Constructor = signature; signature
#endif

namespace detail {

template<typename T> auto MakeArgumentsTuple(ProviderBase&, std::vector<AnyPtr>&);

template<typename T>
concept ConstructorExposed = requires()
{
  std::is_same<typename boost::callable_traits::return_type<typename T::Constructor>::type, T>::value;
};

template<typename T>
concept Creatable = requires()
{
  T::Create;
  std::is_same<typename boost::callable_traits::return_type<decltype(&T::Create)>::type, T*>::value;
};

template<typename T, typename Callable>
concept TypeCreator = requires()
{
  std::is_same<T, typename std::remove_pointer<typename boost::callable_traits::return_type<Callable>::type>::type>::value;
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
    DLOG("Will Create object of type " << util::get_demangled_type_name<T>() << " [Default constructor]");
    Instance ci;
    T *object = new T();
    DLOG("Created object of type " << util::get_demangled_type_name<T>() << " [" << object << "]");
    ci.instance = AnyPtr(new std::any(object), GetDeleter<T>());
    ci.real_instance = ci.instance;
    return ci;
  });
}

template<ConstructorExposed T>
CreatorPtr GetConstructorExposedConstructibleCreator() {
  return Creator::Create([](ProviderBase &provider) {
    DLOG("Will Create object of type " << util::get_demangled_type_name<T>() << " [Exposed constructor]");
    Instance ci;
    using Arguments = typename boost::callable_traits::args<typename T::Constructor>::type;
    auto argument_tuple = MakeArgumentsTuple<Arguments>(provider, ci.dependencies);

    auto call_constructor = [](auto& ...Args){
      return new T(Args...);
    };

    T *object = std::apply(call_constructor, argument_tuple);
    DLOG("Created object of type " << util::get_demangled_type_name<T>() << " [" << object << "]");
    ci.instance = AnyPtr(new std::any(object), GetDeleter<T>());
    ci.real_instance = ci.instance;
    return ci;
  });
}

template<Creatable T>
CreatorPtr GetCreateConstructibleCreator() {
  return Creator::Create([](ProviderBase &provider) {
    DLOG("Will Create object of type " << util::get_demangled_type_name<T>() << " [Create function]");
    Instance ci;
    using Arguments = typename boost::callable_traits::args<decltype(&T::Create)>::type;
    auto argument_tuple = MakeArgumentsTuple<Arguments>(provider, ci.dependencies);

    T *object = std::apply(T::Create, argument_tuple);
    DLOG("Created object of type " << util::get_demangled_type_name<T>() << " [" << object << "]");
    ci.instance = AnyPtr(new std::any(object), GetDeleter<T>());
    ci.real_instance = ci.instance;
    return ci;
  });
}

template<typename Callable>
CreatorPtr GetCallableCreator(Callable f) {
  typedef typename std::remove_pointer<typename boost::callable_traits::return_type<Callable>::type>::type return_type;

  return Creator::Create([f](ProviderBase &provider) {
    DLOG("Will Create object of type " << util::get_demangled_type_name<return_type>() << " [Factory function]");
    Instance ci;
    using Arguments = typename boost::callable_traits::args<Callable>::type;
    auto argument_tuple = MakeArgumentsTuple<Arguments>(provider, ci.dependencies);

    return_type *object = std::apply(f, argument_tuple);
    DLOG("Created object of type " << util::get_demangled_type_name<return_type>() << " [" << object << "]");
    ci.instance = AnyPtr(new std::any(std::move(object)), GetDeleter<return_type>());
    ci.real_instance = ci.instance;
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
    ci.real_instance = ci.instance;
    return ci;
  });
}

} // namespace detail