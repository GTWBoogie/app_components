#pragma once

#include "component_description.h"
#include "component_instance.h"

#include <functional>
#include <map>
#include <mutex>
#include <typeindex>
#include <vector>

class ComponentRegistry;
class ScopedComponentProvider;

class ComponentProvider
{
public:
  ComponentProvider(ComponentRegistry& registry);
  virtual ~ComponentProvider() = default;

  template<typename T>
  T& GetInstance()
  {
    auto type = std::type_index(typeid(T));

    ComponentInstance ci = GetComponentInstance(type);

    return *std::any_cast<T*>(*ci.instance.get());
  }

  template<typename T>
  std::vector<std::reference_wrapper<T>> GetInstances()
  {
    auto type = std::type_index(typeid(T));

    std::vector<std::reference_wrapper<T>> result;

    std::vector<ComponentInstance> cis = GetComponentInstances(type);

    for (auto& ci : cis)
    {
      result.push_back(*std::any_cast<T*>(*ci.instance.get()));
    }

    return result;
  }

  ScopedComponentProvider GetScope();

protected:
  friend class ComponentRegistry;
  friend class ScopedComponentProvider;

  ComponentInstance GetComponentInstance(std::type_index type);
  std::vector<ComponentInstance> GetComponentInstances(std::type_index type);

  virtual ComponentInstance ManageInstanceCreation(ComponentDescription& description);

  std::recursive_mutex _mutex;
  ComponentRegistry& _registry;
  std::map<ComponentCreatorPtr, ComponentInstance> _instances;
  std::vector<ComponentInstance> _floating_instances;  
};

class ScopedComponentProvider : public ComponentProvider
{
protected:
  explicit ScopedComponentProvider(ComponentProvider& parent);

  using ComponentProvider::GetScope;

  friend class ComponentProvider;

  ComponentInstance ManageInstanceCreation(ComponentDescription& description) override;

  ComponentProvider& _parent;
};

