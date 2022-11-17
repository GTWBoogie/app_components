#pragma once

#include "registry_base.h"

#include "creators.h"
#include "converters.h"
#include "detail.h"
#include "description.h"

#include "registry_base.h"

#include <boost/callable_traits.hpp>

class Registry : public RegistryBase {
public:
  Registry() = default;

  template<typename T, typename ...Interfaces>
  void AddInstance(T *object, bool managed = true)
  {
    Register<T, T, Interfaces...>(Lifetime::Singleton, detail::GetAdaptedObjectCreator(object, managed));
  }

  template<typename T, typename ...Interfaces>
  void TryAddInstance(T *object, bool managed = true)
  {
    Register<T, T, Interfaces...>(Lifetime::Singleton, detail::GetAdaptedObjectCreator(object, managed), true);
  }

  template<typename T, typename ...Interfaces>
  void AddInstance(T& object)
  {
    Register<T, T, Interfaces...>(Lifetime::Singleton, detail::GetAdaptedObjectCreator(&object, false));
  }

  template<typename T, typename ...Interfaces>
  void TryAddInstance(T& object)
  {
    Register<T, T, Interfaces...>(Lifetime::Singleton, detail::GetAdaptedObjectCreator(&object, false), true);
  }

  template<typename T, typename ...Interfaces>
  void AddSingleton()
  {
    Register<T, T, Interfaces...>(Lifetime::Singleton, GetCreator<T>());
  }

  template<typename T, typename ...Interfaces, typename Callable>
  void AddSingleton(Callable f) requires detail::TypeCreator<T, Callable>
  {
    Register<T, T, Interfaces...>(Lifetime::Singleton, detail::GetCallableCreator(f));
  }

  template<typename T, typename ...Interfaces>
  void TryAddSingleton()
  {
    Register<T, T, Interfaces...>(Lifetime::Singleton, GetCreator<T>(), true);
  }

  template<typename T, typename ...Interfaces, typename Callable>
  void TryAddSingleton(Callable f) requires detail::TypeCreator<T, Callable>
  {
    Register<T, T, Interfaces...>(Lifetime::Singleton, detail::GetCallableCreator(f), true);
  }

  template<typename T, typename ...Interfaces>
  void AddScoped()
  {
    Register<T, T, Interfaces...>(Lifetime::Scoped, GetCreator<T>());
  }

  template<typename T, typename ...Interfaces, typename Callable>
  void AddScoped(Callable f) requires detail::TypeCreator<T, Callable>
  {
    Register<T, T, Interfaces...>(Lifetime::Scoped, detail::GetCallableCreator(f));
  }

  template<typename T, typename ...Interfaces>
  void TryAddScoped()
  {
    Register<T, T, Interfaces...>(Lifetime::Scoped, GetCreator<T>(), true);
  }

  template<typename T, typename ...Interfaces, typename Callable>
  void TryAddScoped(Callable f) requires detail::TypeCreator<T, Callable>
  {
    Register<T, T, Interfaces...>(Lifetime::Scoped, detail::GetCallableCreator(f), true);
  }

  template<typename T, typename ...Interfaces>
  void AddTransient()
  {
    Register<T, T, Interfaces...>(Lifetime::Transient, GetCreator<T>());
  }

  template<typename T, typename ...Interfaces, typename Callable>
  void AddTransient(Callable f) requires detail::TypeCreator<T, Callable>
  {
    Register<T, T, Interfaces...>(Lifetime::Transient, detail::GetCallableCreator(f));
  }

  template<typename T, typename ...Interfaces>
  void TryAddTransient()
  {
    Register<T, T, Interfaces...>(Lifetime::Transient, GetCreator<T>(), true);
  }

  template<typename T, typename ...Interfaces, typename Callable>
  void TryAddTransient(Callable f) requires detail::TypeCreator<T, Callable>
  {
    Register<T, T, Interfaces...>(Lifetime::Transient, detail::GetCallableCreator(f), true);
  }

protected:
  template<std::default_initializable T> CreatorPtr GetCreator() requires (!detail::Creatable<T>)
  {
    return detail::GetDefaultConstructibleCreator<T>();
  }

  template<detail::ConstructorExposed T> CreatorPtr GetCreator() requires (!std::default_initializable<T> && !detail::Creatable<T>)
  {
    return detail::GetConstructorExposedConstructibleCreator<T>();
  }

  template<detail::Creatable T> CreatorPtr GetCreator()
  {
    return detail::GetCreateConstructibleCreator<T>();
  }

  template<typename From, typename To, typename ...Other>
  void Register(Lifetime lifetime, CreatorPtr creator, bool unique = false)
  {
    std::type_index type = (typeid(To));

    RegistryBase::Register(lifetime, type, typeid(From), creator, detail::GetConverter<From, To>(), unique);

    if constexpr (sizeof...(Other) > 0)
    {
      Register<From, Other...>(lifetime, creator, unique);
    }
  }
};

