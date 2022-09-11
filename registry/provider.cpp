#include "provider.h"

#include "description.h"
#include "registry.h"

Provider::Provider(Registry& registry)
 : _registry(registry)
{
}

Instance Provider::GetComponentInstance(std::type_index type)
{
  Description& description = _registry.GetDescriptions(type).back();

  std::lock_guard<std::recursive_mutex> lock(_mutex);
  return ManageInstanceCreation(description);
}

std::vector<Instance> Provider::GetComponentInstances(std::type_index type)
{
  auto& descriptions = _registry.GetDescriptions(type);
  std::vector<Instance> result;
  result.reserve(descriptions.size());

  std::lock_guard<std::recursive_mutex> lock(_mutex);
  for (auto& description : descriptions)
  {
    result.push_back(ManageInstanceCreation(description));
  }

  return result;
}

Instance Provider::ManageInstanceCreation(Description& description)
{
  switch (description.GetLifetime())
  {
    case Lifetime::Transient:
      {
        Instance ci = description.GetCreator()->CreateInstance(*this);
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
          Instance ci = it->second;
          ci.instance = description.GetConverter().Convert(ci.instance);
          return ci;
        }
        Instance ci = description.GetCreator()->CreateInstance(*this);
        _instances[description.GetCreator()] = ci;
        ci.instance = description.GetConverter().Convert(ci.instance);
        return ci;
      }
  }
}

ScopedProvider Provider::GetScope()
{
  return ScopedProvider(*this);
}

ScopedProvider::ScopedProvider(Provider& parent)
 : Provider(parent._registry)
 , _parent(parent)
{
  _registry.AddInstance<Provider>(this, false);
}

Instance ScopedProvider::ManageInstanceCreation(Description& description)
{
  switch (description.GetLifetime())
  {
    case Lifetime::Transient:
      {
        Instance ci = description.GetCreator()->CreateInstance(*this);
        _floating_instances.push_back(ci);
        ci.instance = description.GetConverter().Convert(ci.instance);
        return ci;
      }
    case Lifetime::Scoped:
      {
        auto it = _instances.find(description.GetCreator());
        if (it != _instances.end())
        {
          Instance ci = it->second;
          ci.instance = description.GetConverter().Convert(ci.instance);
          return ci;
        }
        Instance ci = description.GetCreator()->CreateInstance(*this);
        _instances[description.GetCreator()] = ci;
        ci.instance = description.GetConverter().Convert(ci.instance);
        return ci;
      }
    case Lifetime::Singleton:
      {
        return _parent.ManageInstanceCreation(description);
      }
  }
}

