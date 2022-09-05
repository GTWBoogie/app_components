#pragma once

#include "component_instance.h"
#include "component_description.h"
#include "component_provider.h"

#include "util/tuple.h"

#include <boost/callable_traits.hpp>
#include <boost/core/demangle.hpp>

#include <iostream>
#include <typeindex>
#include <type_traits>

#include <any>
#include <concepts>
#include <functional>
#include <memory>
#include <map>
#include <vector>

template<typename T>
concept Creatable = requires()
{
  T::Create;
  std::is_same<typename boost::callable_traits::return_type<decltype(&T::Create)>::type, T*>::value;
};

class ComponentRegistry
{
public:
  ComponentRegistry()
  {
  }

  template<typename ...Interfaces, typename T>
  void AddInstance(T *object, bool managed = true)
  {
    {
      std::cout << "Adapted object of type " << boost::core::demangle(typeid(T).name()) << " [" << object << "]" << std::endl;
    }

    AnyPtr object_pointer;
    if (managed)
    {
      object_pointer = AnyPtr(new std::any(std::move(object)), GetDeleter<T>());
    }
    else
    {
      object_pointer = std::make_shared<std::any>(std::move(object));
    }

    auto creator = ComponentCreator::Create(
      [object_pointer](ComponentProvider&) {
        ComponentInstance ci;
        ci.instance = object_pointer;
        return ci;
      });

    Register<T, T, Interfaces...>(Lifetime::Scoped, creator);
  }

  template<typename T, typename ...Interfaces>
  void AddSingleton() requires std::default_initializable<T>
  {
    Register<T, T, Interfaces...>(Lifetime::Singleton, GetDefaultConstructibleCreator<T>());
  }

  template<typename T, typename ...Interfaces>
  void AddSingleton() requires Creatable<T>
  {
    Register<T, T, Interfaces...>(Lifetime::Singleton, GetCreateConstructibleCreator<T>());
  }

  template<typename ...Interfaces, typename Callable>
  void AddSingleton(Callable f)
  {
    typedef typename std::remove_pointer<typename boost::callable_traits::return_type<Callable>::type>::type return_type;
    Register<return_type, return_type, Interfaces...>(Lifetime::Singleton, GetCreator(f));
  }

  template<typename T, typename ...Interfaces>
  void TryAddSingleton() requires std::default_initializable<T>
  {
    Register<T, T, Interfaces...>(Lifetime::Singleton, GetDefaultConstructibleCreator<T>(), true);
  }

  template<typename ...Interfaces, typename Callable>
  void TryAddSingleton(Callable f)
  {
    typedef typename std::remove_pointer<typename boost::callable_traits::return_type<Callable>::type>::type return_type;
    Register<return_type, return_type, Interfaces...>(Lifetime::Singleton, GetCreator(f), true);
  }

  template<typename T, typename ...Interfaces>
  void AddScoped() requires std::default_initializable<T>
  {
    Register<T, T, Interfaces...>(Lifetime::Scoped, GetDefaultConstructibleCreator<T>());
  }

  template<typename ...Interfaces, typename Callable>
  void AddScoped(Callable f)
  {
    typedef typename std::remove_pointer<typename boost::callable_traits::return_type<Callable>::type>::type return_type;
    Register<return_type, return_type, Interfaces...>(Lifetime::Scoped, GetCreator(f));
  }

  template<typename T, typename ...Interfaces>
  void TryAddScoped() requires std::default_initializable<T>
  {
    Register<T, T, Interfaces...>(Lifetime::Scoped, GetDefaultConstructibleCreator<T>(), true);
  }

  template<typename ...Interfaces, typename Callable>
  void TryAddScoped(Callable f)
  {
    typedef typename std::remove_pointer<typename boost::callable_traits::return_type<Callable>::type>::type return_type;
    Register<return_type, return_type, Interfaces...>(Lifetime::Scoped, GetCreator(f), true);
  }

  template<typename T, typename ...Interfaces>
  void AddTransient() requires std::default_initializable<T>
  {
    Register<T, T, Interfaces...>(Lifetime::Transient, GetDefaultConstructibleCreator<T>());
  }

  template<typename ...Interfaces, typename Callable>
  void AddTransient(Callable f)
  {
    typedef typename std::remove_pointer<typename boost::callable_traits::return_type<Callable>::type>::type return_type;
    Register<return_type, return_type, Interfaces...>(Lifetime::Transient, GetCreator(f));
  }

  template<typename T, typename ...Interfaces>
  void TryAddTransient() requires std::default_initializable<T>
  {
    Register<T, T, Interfaces...>(Lifetime::Transient, GetDefaultConstructibleCreator<T>(), true);
  }

  template<typename ...Interfaces, typename Callable>
  void TryAddTransient(Callable f)
  {
    typedef typename std::remove_pointer<typename boost::callable_traits::return_type<Callable>::type>::type return_type;
    Register<return_type, return_type, Interfaces...>(Lifetime::Transient, GetCreator(f), true);
  }

protected:
  friend class ComponentProvider;

  std::vector<ComponentDescription>& GetDescriptions(std::type_index type)
  {
    auto it = _descriptions.find(type);
    if (it == _descriptions.end())
    {
      throw std::invalid_argument(std::string("Object providing interface ") + boost::core::demangle(type.name()) + " not registered");
    }

    return it->second;
  }

  template<typename From, typename To, typename ...Other>
  void Register(Lifetime lifetime, ComponentCreatorPtr creator, bool unique = false)
  {
    std::type_index type = (typeid(To));

    auto it = _descriptions.find(type);
    if (unique && it != _descriptions.end())
    {
      std::cout << "Interface " << boost::core::demangle(typeid(To).name()) << " already registered - skipping registering for class " << boost::core::demangle(typeid(From).name()) << std::endl;
    }
    else
    {
      _descriptions[type].push_back(ComponentDescription(lifetime, creator, GetConverter<From, To>()));
      std::cout << "Registered interface " << boost::core::demangle(typeid(To).name()) << " for class " << boost::core::demangle(typeid(From).name()) << std::endl;
    }

    if constexpr (sizeof...(Other) > 0)
    {
      Register<From, Other...>(lifetime, creator, unique);
    }
  }

  template<typename ObjectType>
  static auto GetDeleter()
  {
    return [](std::any *ptr) {
      ObjectType *pointer = std::any_cast<ObjectType *>(*ptr);
      {
        std::cout << "Deleting object of type " << boost::core::demangle(typeid(ObjectType).name()) << " [" << pointer << "]" << std::endl;
      }
      delete pointer;
      delete ptr;
    };
  }

  template<typename From, typename To>
  static ComponentConverter GetConverter()
  {
    if constexpr (std::is_same<From, To>::value)
    {
      return ComponentConverter([](AnyPtr ptr) { return ptr; });
    }
    else
    {
      return ComponentConverter([](AnyPtr ptr) {
        To *to = dynamic_cast<To*>(std::any_cast<From*>(*ptr.get()));
        if (to == nullptr)
        {
          throw std::invalid_argument(std::string("Object of type " + boost::core::demangle(typeid(From).name()) +
                " does not provide interface ") + boost::core::demangle(typeid(To).name()));
        }
        return AnyPtr(new std::any(to));
      });
    }
  }

  template<typename T>
  static ComponentCreatorPtr GetDefaultConstructibleCreator()
  {
    return ComponentCreator::Create([](ComponentProvider&) {
      ComponentInstance ci;
      T* object = new T();
      {
        std::cout << "Created object of type " << boost::core::demangle(typeid(T).name()) << " [" << object << "]" << std::endl;
      }
      ci.instance = AnyPtr(new std::any(object), GetDeleter<T>());
      return ci;
    });
  }

  template<typename T>
  static ComponentCreatorPtr GetCreateConstructibleCreator()
  {
    return ComponentCreator::Create([](ComponentProvider& provider) {
      ComponentInstance ci;
      using Arguments = typename boost::callable_traits::args<decltype(&T::Create)>::type;
      Arguments argument_tuple = MakeArgumentsTuple<Arguments>(provider, ci.dependencies);

      T* object = std::apply(T::Create, argument_tuple);
      {
        std::cout << "Created object of type " << boost::core::demangle(typeid(T).name()) << " [" << object << "]" << std::endl;
      }
      ci.instance = AnyPtr(new std::any(object), GetDeleter<T>());
      return ci;
    });
  }

  template<typename Callable>
  static ComponentCreatorPtr GetCreator(Callable f)
  {
    typedef typename std::remove_pointer<typename boost::callable_traits::return_type<Callable>::type>::type return_type;

    return ComponentCreator::Create([f](ComponentProvider& provider) {
      ComponentInstance ci;
      using Arguments = typename boost::callable_traits::args<Callable>::type;
      Arguments argument_tuple = MakeArgumentsTuple<Arguments>(provider, ci.dependencies);

      return_type * object = std::apply(f, argument_tuple);
      {
        std::cout << "Created object of type " << boost::core::demangle(typeid(return_type).name()) << " [" << object << "]" << std::endl;
      }
      ci.instance = AnyPtr(new std::any(std::move(object)), GetDeleter<return_type>());
      return ci;
    });
  }

  template<typename T>
  static T CreateArgumentsTuple(ComponentProvider& cp, std::vector<AnyPtr>& dependencies)
  {
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

      return std::tuple_cat(std::tuple<type&>(*std::any_cast<type *>(*instance.instance)), CreateArgumentsTuple<typename util::tuple_trunc<1, T>::type>(cp, dependencies));
    }
  }

  template<typename T>
  static T MakeArgumentsTuple(ComponentProvider& cp, std::vector<AnyPtr>& dependencies)
  {
    return CreateArgumentsTuple<T>(cp, dependencies);
  }

  std::map<std::type_index, std::vector<ComponentDescription>> _descriptions;
};

