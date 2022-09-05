#include "component_provider.h"

#include "component_registry.h"

ComponentProvider::ComponentProvider(ComponentRegistry& registry)
 : _registry(registry)
{
}

ComponentInstance ComponentProvider::GetComponentInstance(std::type_index type)
{
  ComponentDescription& description = _registry.GetDescriptions(type).back();

  std::lock_guard<std::recursive_mutex> lock(_mutex);
  return ManageInstanceCreation(description);
}

std::vector<ComponentInstance> ComponentProvider::GetComponentInstances(std::type_index type)
{
  auto& descriptions = _registry.GetDescriptions(type);
  std::vector<ComponentInstance> result;
  result.reserve(descriptions.size());

  std::lock_guard<std::recursive_mutex> lock(_mutex);
  for (auto& description : descriptions)
  {
    result.push_back(ManageInstanceCreation(description));
  }

  return result;
}

ComponentInstance ComponentProvider::ManageInstanceCreation(ComponentDescription& description)
{
  switch (description.GetLifetime())
  {
    case Lifetime::Transient:
      {
        ComponentInstance ci = description.GetCreator()->CreateInstance(*this);
        _floating_instances.push_back(ci);
        ci.instance = description.GetConverter().Convert(ci.instance);
        return ci;
      }
    case Lifetime::Scoped:
    case Lifetime::Singleton:
      {
        auto it = _instances.find(description.GetCreator());
        if (it != _instances.end())
        {
          ComponentInstance ci = it->second;
          ci.instance = description.GetConverter().Convert(ci.instance);
          return ci;
        }
        ComponentInstance ci = description.GetCreator()->CreateInstance(*this);
        _instances[description.GetCreator()] = ci;
        ci.instance = description.GetConverter().Convert(ci.instance);
        return ci;
      }
  }
}

ScopedComponentProvider ComponentProvider::GetScope()
{
  return ScopedComponentProvider(*this);
}

ScopedComponentProvider::ScopedComponentProvider(ComponentProvider& parent)
 : ComponentProvider(parent._registry) 
 , _parent(parent)
{
  _registry.AddInstance<ComponentProvider>(this, false);
}

ComponentInstance ScopedComponentProvider::ManageInstanceCreation(ComponentDescription& description)
{
  switch (description.GetLifetime())
  {
    case Lifetime::Transient:
      {
        ComponentInstance ci = description.GetCreator()->CreateInstance(*this);
        _floating_instances.push_back(ci);
        ci.instance = description.GetConverter().Convert(ci.instance);
        return ci;
      }
    case Lifetime::Scoped:
      {
      {
        auto it = _instances.find(description.GetCreator());
        if (it != _instances.end())
        {
          ComponentInstance ci = it->second;
          ci.instance = description.GetConverter().Convert(ci.instance);
          return ci;
        }
        ComponentInstance ci = description.GetCreator()->CreateInstance(*this);
        _instances[description.GetCreator()] = ci;
        ci.instance = description.GetConverter().Convert(ci.instance);
        return ci;
      }
      }
    case Lifetime::Singleton:
      {
        return _parent.ManageInstanceCreation(description);
      }
  }
}

