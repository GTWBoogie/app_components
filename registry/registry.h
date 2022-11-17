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

  template<std::default_initializable T, typename ...Interfaces>
  void AddSingleton() requires (!detail::Creatable<T>)
  {
    Register<T, T, Interfaces...>(Lifetime::Singleton, detail::GetDefaultConstructibleCreator<T>());
  }

  template<detail::ConstructorExposed T, typename ...Interfaces>
  void AddSingleton() requires (!std::default_initializable<T> && !detail::Creatable<T>)
  {
    Register<T, T, Interfaces...>(Lifetime::Singleton, detail::GetConstructorExposedConstructibleCreator<T>());
  }

  template<detail::Creatable T, typename ...Interfaces>
  void AddSingleton()
  {
    Register<T, T, Interfaces...>(Lifetime::Singleton, detail::GetCreateConstructibleCreator<T>());
  }

  template<typename T, typename ...Interfaces, typename Callable>
  void AddSingleton(Callable f) requires detail::TypeCreator<T, Callable>
  {
    Register<T, T, Interfaces...>(Lifetime::Singleton, detail::GetCallableCreator(f));
  }

  template<std::default_initializable T, typename ...Interfaces>
  void TryAddSingleton() requires (!detail::Creatable<T>)
  {
    Register<T, T, Interfaces...>(Lifetime::Singleton, detail::GetDefaultConstructibleCreator<T>(), true);
  }

  template<detail::ConstructorExposed T, typename ...Interfaces>
  void TryAddSingleton() requires (!std::default_initializable<T> && !detail::Creatable<T>)
  {
    Register<T, T, Interfaces...>(Lifetime::Singleton, detail::GetConstructorExposedConstructibleCreator<T>(), true);
  }

  template<detail::Creatable T, typename ...Interfaces>
  void TryAddSingleton()
  {
    Register<T, T, Interfaces...>(Lifetime::Singleton, detail::GetCreateConstructibleCreator<T>(), true);
  }

  template<typename T, typename ...Interfaces, typename Callable>
  void TryAddSingleton(Callable f) requires detail::TypeCreator<T, Callable>
  {
    Register<T, T, Interfaces...>(Lifetime::Singleton, detail::GetCallableCreator(f), true);
  }

  template<std::default_initializable T, typename ...Interfaces>
  void AddScoped() requires (!detail::Creatable<T>)
  {
    Register<T, T, Interfaces...>(Lifetime::Scoped, detail::GetDefaultConstructibleCreator<T>());
  }

  template<detail::ConstructorExposed T, typename ...Interfaces>
  void AddScoped() requires (!std::default_initializable<T> && !detail::Creatable<T>)
  {
    Register<T, T, Interfaces...>(Lifetime::Scoped, detail::GetConstructorExposedConstructibleCreator<T>());
  }

  template<detail::Creatable T, typename ...Interfaces>
  void AddScoped()
  {
    Register<T, T, Interfaces...>(Lifetime::Scoped, detail::GetCreateConstructibleCreator<T>());
  }

  template<typename T, typename ...Interfaces, typename Callable>
  void AddScoped(Callable f) requires detail::TypeCreator<T, Callable>
  {
    Register<T, T, Interfaces...>(Lifetime::Scoped, detail::GetCallableCreator(f));
  }

  template<std::default_initializable T, typename ...Interfaces>
  void TryAddScoped() requires (!detail::Creatable<T>)
  {
    Register<T, T, Interfaces...>(Lifetime::Scoped, detail::GetDefaultConstructibleCreator<T>(), true);
  }

  template<detail::ConstructorExposed T, typename ...Interfaces>
  void TryAddScoped() requires (!std::default_initializable<T> && !detail::Creatable<T>)
  {
    Register<T, T, Interfaces...>(Lifetime::Scoped, detail::GetConstructorExposedConstructibleCreator<T>(), true);
  }

  template<detail::Creatable T, typename ...Interfaces>
  void TryAddScoped()
  {
    Register<T, T, Interfaces...>(Lifetime::Scoped, detail::GetCreateConstructibleCreator<T>(), true);
  }

  template<typename T, typename ...Interfaces, typename Callable>
  void TryAddScoped(Callable f) requires detail::TypeCreator<T, Callable>
  {
    Register<T, T, Interfaces...>(Lifetime::Scoped, detail::GetCallableCreator(f), true);
  }

  template<std::default_initializable T, typename ...Interfaces>
  void AddTransient() requires (!detail::Creatable<T>)
  {
    Register<T, T, Interfaces...>(Lifetime::Transient, detail::GetDefaultConstructibleCreator<T>());
  }

  template<detail::ConstructorExposed T, typename ...Interfaces>
  void AddTransient() requires (!std::default_initializable<T> && !detail::Creatable<T>)
  {
    Register<T, T, Interfaces...>(Lifetime::Transient, detail::GetConstructorExposedConstructibleCreator<T>());
  }

  template<detail::Creatable T, typename ...Interfaces>
  void AddTransient()
  {
    Register<T, T, Interfaces...>(Lifetime::Transient, detail::GetCreateConstructibleCreator<T>());
  }

  template<typename T, typename ...Interfaces, typename Callable>
  void AddTransient(Callable f) requires detail::TypeCreator<T, Callable>
  {
    Register<T, T, Interfaces...>(Lifetime::Transient, detail::GetCallableCreator(f));
  }

  template<std::default_initializable T, typename ...Interfaces>
  void TryAddTransient() requires (!detail::Creatable<T>)
  {
    Register<T, T, Interfaces...>(Lifetime::Transient, detail::GetDefaultConstructibleCreator<T>(), true);
  }

  template<detail::ConstructorExposed T, typename ...Interfaces>
  void TryAddTransient() requires (!std::default_initializable<T> && !detail::Creatable<T>)
  {
    Register<T, T, Interfaces...>(Lifetime::Transient, detail::GetConstructorExposedConstructibleCreator<T>(), true);
  }

  template<detail::Creatable T, typename ...Interfaces>
  void TryAddTransient()
  {
    Register<T, T, Interfaces...>(Lifetime::Transient, detail::GetCreateConstructibleCreator<T>(), true);
  }

  template<typename T, typename ...Interfaces, typename Callable>
  void TryAddTransient(Callable f) requires detail::TypeCreator<T, Callable>
  {
    Register<T, T, Interfaces...>(Lifetime::Transient, detail::GetCallableCreator(f), true);
  }

protected:
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

