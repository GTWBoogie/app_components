#pragma once

#include "instance.h"
#include "tagged_type.h"

#include <map>
#include <mutex>
#include <typeindex>
#include <vector>

class Description;
class Provider;
class RegistryBase;
class ScopedProvider;

class Creator;
using CreatorPtr = std::shared_ptr<Creator>;

namespace detail {

template<typename T> auto MakeArgumentsTuple(Provider&, std::vector<AnyPtr>&);

} // namespace detail

template<typename T>
using ComponentContainer = std::vector<std::reference_wrapper<T>>;

class Provider
{
public:
  Provider(RegistryBase& registry);
  virtual ~Provider() = default;

  template<typename T>
  T& GetInstance()
  {
    Instance ci = GetComponentInstance(typeid(T));
    return *std::any_cast<T*>(*ci.instance.get());
  }

  template<TaggedType T>
  T& GetInstance()
  {
    Instance ci = GetComponentInstance(typeid(typename T::type), typeid(typename T::tag));
    return T(*std::any_cast<typename T::type*>(*ci.instance.get()));
  }

  template<typename T>
  ComponentContainer<T> GetInstances()
  {
    std::vector<std::reference_wrapper<T>> result;
    std::vector<Instance> cis = GetComponentInstances(typeid(T));
    for (auto& ci : cis)
      result.push_back(*std::any_cast<T*>(*ci.instance.get()));

    return result;
  }

  template<typename T>
  Tagged<ComponentContainer<typename T::type>, typename T::tag> GetInstances()
  {
    std::vector<std::reference_wrapper<typename T::type>> result;
    std::vector<Instance> cis = GetComponentInstances(typeid(typename T::type), std::type_index(typeid(typename T::tag)));
    for (auto& ci : cis)
      result.push_back(*std::any_cast<typename T::type*>(*ci.instance.get()));

    return T(result);
  }
  ScopedProvider GetScope();

  Instance GetComponentInstance(std::type_index type);
  Instance GetComponentInstance(std::type_index type, std::type_index tag);

protected:
  friend class ScopedProvider;

  std::vector<Instance> GetComponentInstances(std::type_index type);
  std::vector<Instance> GetComponentInstances(std::type_index type, std::type_index tag);

  virtual Instance ManageInstanceCreation(Description& description, std::type_index tag);

  std::recursive_mutex _mutex;
  RegistryBase& _registry;
  std::map<std::pair<std::type_index, CreatorPtr>, Instance> _instances;
  std::vector<Instance> _floating_instances;
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
