#pragma once

#include "detail.h"
#include "description.h"

#include "util/debug_log.h"
#include "util/demangle.h"

#include <any>
#include <memory>
#include <map>
#include <vector>

class Registry
{
public:
  Registry() = default;

  template<typename ...Interfaces, typename T>
  void AddInstance(T *object, bool managed = true)
  {
    Register<T, T, Interfaces...>(Lifetime::Scoped, detail::GetAdaptedObjectCreator(object, managed));
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
  friend class Provider;

  std::vector<Description>& GetDescriptions(std::type_index type)
  {
    auto it = _descriptions.find(type);
    if (it == _descriptions.end())
    {
      throw std::invalid_argument(std::string("Object providing interface ") + util::get_demangled_type_name(type.name()) + " not registered");
    }

    return it->second;
  }

  template<typename From, typename To, typename ...Other>
  void Register(Lifetime lifetime, CreatorPtr creator, bool unique = false)
  {
    std::type_index type = (typeid(To));

    auto it = _descriptions.find(type);
    if (unique && it != _descriptions.end())
    {
      DLOG("Interface " << util::get_demangled_type_name<To>()
              << " already registered - skipping registering for class " << util::get_demangled_type_name<From>());
    }
    else
    {
      _descriptions[type].push_back(Description(lifetime, creator, detail::GetConverter<From, To>()));
      DLOG("Registered interface " << util::get_demangled_type_name<To>()
              << " for class " << util::get_demangled_type_name<From>());
    }

    if constexpr (sizeof...(Other) > 0)
    {
      Register<From, Other...>(lifetime, creator, unique);
    }
  }

  std::map<std::type_index, std::vector<Description>> _descriptions;
};

