#pragma once

#include "description.h"
#include "instance.h"

#include <functional>
#include <map>
#include <mutex>
#include <typeindex>
#include <vector>

class Registry;
class ScopedProvider;

class Provider
{
public:
  Provider(Registry& registry);
  virtual ~Provider() = default;

  template<typename T>
  T& GetInstance()
  {
    auto type = std::type_index(typeid(T));

    Instance ci = GetComponentInstance(type);

    return *std::any_cast<T*>(*ci.instance.get());
  }

  template<typename T>
  std::vector<std::reference_wrapper<T>> GetInstances()
  {
    auto type = std::type_index(typeid(T));

    std::vector<std::reference_wrapper<T>> result;

    std::vector<Instance> cis = GetComponentInstances(type);

    for (auto& ci : cis)
    {
      result.push_back(*std::any_cast<T*>(*ci.instance.get()));
    }

    return result;
  }

  ScopedProvider GetScope();

protected:
  friend class Registry;
  friend class ScopedProvider;

  Instance GetComponentInstance(std::type_index type);
  std::vector<Instance> GetComponentInstances(std::type_index type);

  virtual Instance ManageInstanceCreation(Description& description);

  std::recursive_mutex _mutex;
  Registry& _registry;
  std::map<ComponentCreatorPtr, Instance> _instances;
  std::vector<Instance> _floating_instances;
};

class ScopedProvider : public Provider
{
protected:
  explicit ScopedProvider(Provider& parent);

  using Provider::GetScope;

  friend class Provider;

  Instance ManageInstanceCreation(Description& description) override;

  Provider& _parent;
};
