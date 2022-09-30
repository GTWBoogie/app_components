#pragma once

#include "provider_base.h"

#include "instance.h"
#include "tagged_type.h"

class Description;
class Provider;
class RegistryBase;
class ScopedProvider;

template<typename T>
using ComponentContainer = std::vector<std::reference_wrapper<T>>;

class Provider : public ProviderBase
{
public:
  explicit Provider(RegistryBase& registry);
  ~Provider() override = default;

  template<typename T>
  T& GetInstance()
  {
    Instance ci = GetComponentInstance(typeid(T));
    return *std::any_cast<T*>(*ci.instance.get());
  }

  template<TaggedType T>
  T GetInstance()
  {
    Instance ci = GetComponentInstance(typeid(typename T::type), typeid(typename T::tag));
    return T(**std::any_cast<typename T::type*>(ci.instance.get()));
  }

  template<typename T>
  ComponentContainer<T> GetInstances()
  {
    ComponentContainer<T> result;
    std::vector<Instance> cis = GetComponentInstances(typeid(T));
    for (auto& ci : cis)
      result.push_back(*std::any_cast<T*>(*ci.instance.get()));

    return result;
  }

  template<typename T>
  Tagged<ComponentContainer<typename T::type>, typename T::tag> GetInstances()
  {
    ComponentContainer<typename T::type> result;
    std::vector<Instance> cis = GetComponentInstances(typeid(typename T::type), std::type_index(typeid(typename T::tag)));
    for (auto& ci : cis)
      result.push_back(*std::any_cast<typename T::type*>(*ci.instance.get()));

    return T(result);
  }
  ScopedProvider GetScope();

protected:
  friend class ScopedProvider;

  Instance ManageInstanceCreation(Description& description, std::type_index tag) override;
};

class ScopedProvider : public Provider
{
protected:
  explicit ScopedProvider(Provider& parent);

  using Provider::GetScope;

  friend class Provider;

  Instance ManageInstanceCreation(Description& description, std::type_index tag) override;

  Provider& _parent;
};
