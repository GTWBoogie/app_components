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
struct ConverterCreator {
  static Converter Create()
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
};

template<typename From>
struct ConverterCreator<From, From> {
  static Converter Create()
  {
    return Converter([](const AnyPtr& ptr) { return ptr; });
  }
};

template<typename From, TaggedType To>
struct ConverterCreator<From, To> {
  static Converter Create()
  {
    return Converter([](const AnyPtr& ptr) {
      typename To::type *to = dynamic_cast<typename To::type*>(std::any_cast<From*>(*ptr.get()));
      if (to == nullptr)
      {
        throw std::invalid_argument(std::string("Object of type " + util::get_demangled_type_name<From>() +
                                                " does not provide interface ") +
                                                util::get_demangled_type_name<typename To::type>());
      }
      return AnyPtr(new std::any(to));
    });
  }
};

template<typename From, typename To>
Converter GetConverter()
{
  return ConverterCreator<From, To>::Create();
}

template<typename Type>
struct ComponentInstanceGetter {
  static Instance Get(Provider& cp)
  {
    return cp.GetComponentInstance(std::type_index(typeid(Type)));
  }
};

template<TaggedType Type>
struct ComponentInstanceGetter<Type> {
  static Instance Get(Provider& cp)
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
auto MakeArgumentsTuple(Provider& cp, std::vector<AnyPtr>& dependencies)
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
    DLOG("Will Create object of type " << util::get_demangled_type_name<T>());
    Instance ci;
    using Arguments = typename boost::callable_traits::args<decltype(&T::Create)>::type;
    auto argument_tuple = MakeArgumentsTuple<Arguments>(provider, ci.dependencies);

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
    auto argument_tuple = MakeArgumentsTuple<Arguments>(provider, ci.dependencies);

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
