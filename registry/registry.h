#pragma once

#include "registry_base.h"

#include "creators.h"
#include "converters.h"
#include "detail.h"
#include "description.h"

#include "util/demangle.h"
#include "registry_base.h"

#include <any>
#include <memory>
#include <map>
#include <vector>

class Registry : public RegistryBase {
public:
  Registry() = default;

  template<typename ...Interfaces, typename T>
  void AddInstance(T *object, bool managed = true)
  {
    Register<T, T, Interfaces...>(Lifetime::Scoped, detail::GetAdaptedObjectCreator(object, managed));
  }

  template<typename ...Interfaces, typename T>
  void TryAddInstance(T *object, bool managed = true)
  {
    Register<T, T, Interfaces...>(Lifetime::Scoped, detail::GetAdaptedObjectCreator(object, managed), true);
  }

  template<typename T, typename ...Interfaces>
  void AddSingleton() requires std::default_initializable<T>
  {
    Register<T, T, Interfaces...>(Lifetime::Singleton, detail::GetDefaultConstructibleCreator<T>());
  }

  template<typename T, typename ...Interfaces>
  void AddSingleton() requires detail::Creatable<T>
  {
    Register<T, T, Interfaces...>(Lifetime::Singleton, detail::GetCreateConstructibleCreator<T>());
  }

  template<typename ...Interfaces, typename Callable>
  void AddSingleton(Callable f)
  {
    typedef typename std::remove_pointer<typename boost::callable_traits::return_type<Callable>::type>::type return_type;
    Register<return_type, return_type, Interfaces...>(Lifetime::Singleton, detail::GetCreator(f));
  }

  template<typename T, typename ...Interfaces>
  void TryAddSingleton() requires std::default_initializable<T>
  {
    Register<T, T, Interfaces...>(Lifetime::Singleton, detail::GetDefaultConstructibleCreator<T>(), true);
  }

  template<typename T, typename ...Interfaces>
  void TryAddSingleton() requires detail::Creatable<T>
  {
    Register<T, T, Interfaces...>(Lifetime::Singleton, detail::GetCreateConstructibleCreator<T>(), true);
  }

  template<typename ...Interfaces, typename Callable>
  void TryAddSingleton(Callable f)
  {
    typedef typename std::remove_pointer<typename boost::callable_traits::return_type<Callable>::type>::type return_type;
    Register<return_type, return_type, Interfaces...>(Lifetime::Singleton, detail::GetCreator(f), true);
  }

  template<typename T, typename ...Interfaces>
  void AddScoped() requires std::default_initializable<T>
  {
    Register<T, T, Interfaces...>(Lifetime::Scoped, detail::GetDefaultConstructibleCreator<T>());
  }

  template<typename T, typename ...Interfaces>
  void AddScoped() requires detail::Creatable<T>
  {
    Register<T, T, Interfaces...>(Lifetime::Scoped, detail::GetCreateConstructibleCreator<T>());
  }

  template<typename ...Interfaces, typename Callable>
  void AddScoped(Callable f)
  {
    typedef typename std::remove_pointer<typename boost::callable_traits::return_type<Callable>::type>::type return_type;
    Register<return_type, return_type, Interfaces...>(Lifetime::Scoped, detail::GetCreator(f));
  }

  template<typename T, typename ...Interfaces>
  void TryAddScoped() requires std::default_initializable<T>
  {
    Register<T, T, Interfaces...>(Lifetime::Scoped, detail::GetDefaultConstructibleCreator<T>(), true);
  }

  template<typename T, typename ...Interfaces>
  void TryAddScoped() requires detail::Creatable<T>
  {
    Register<T, T, Interfaces...>(Lifetime::Scoped, detail::GetCreateConstructibleCreator<T>(), true);
  }

  template<typename ...Interfaces, typename Callable>
  void TryAddScoped(Callable f)
  {
    typedef typename std::remove_pointer<typename boost::callable_traits::return_type<Callable>::type>::type return_type;
    Register<return_type, return_type, Interfaces...>(Lifetime::Scoped, detail::GetCreator(f), true);
  }

  template<typename T, typename ...Interfaces>
  void AddTransient() requires std::default_initializable<T>
  {
    Register<T, T, Interfaces...>(Lifetime::Transient, detail::GetDefaultConstructibleCreator<T>());
  }

  template<typename T, typename ...Interfaces>
  void AddTransient() requires detail::Creatable<T>
  {
    Register<T, T, Interfaces...>(Lifetime::Transient, detail::GetCreateConstructibleCreator<T>());
  }

  template<typename ...Interfaces, typename Callable>
  void AddTransient(Callable f)
  {
    typedef typename std::remove_pointer<typename boost::callable_traits::return_type<Callable>::type>::type return_type;
    Register<return_type, return_type, Interfaces...>(Lifetime::Transient, detail::GetCreator(f));
  }

  template<typename T, typename ...Interfaces>
  void TryAddTransient() requires std::default_initializable<T>
  {
    Register<T, T, Interfaces...>(Lifetime::Transient, detail::GetDefaultConstructibleCreator<T>(), true);
  }

  template<typename T, typename ...Interfaces>
  void TryAddTransient() requires detail::Creatable<T>
  {
    Register<T, T, Interfaces...>(Lifetime::Transient, detail::GetCreateConstructibleCreator<T>(), true);
  }

  template<typename ...Interfaces, typename Callable>
  void TryAddTransient(Callable f)
  {
    typedef typename std::remove_pointer<typename boost::callable_traits::return_type<Callable>::type>::type return_type;
    Register<return_type, return_type, Interfaces...>(Lifetime::Transient, detail::GetCreator(f), true);
  }

protected:
  template<typename From, typename To, typename ...Other>
  void Register(Lifetime lifetime, CreatorPtr creator, bool unique = false)
  {
    std::type_index type = (typeid(To));

    RegistryBase::Register(lifetime, type, typeid(From), creator, detail::GetConverter<From, To>());

    if constexpr (sizeof...(Other) > 0)
    {
      Register<From, Other...>(lifetime, creator, unique);
    }
  }
};

