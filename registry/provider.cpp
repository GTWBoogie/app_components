#include "provider.h"

#include "converters.h"
#include "creators.h"
#include "description.h"
#include "registry_base.h"

namespace {

struct ProviderDefaultTag{};
const std::type_index _providerDefaultTagIndex = std::type_index(typeid(ProviderDefaultTag));

} // namespace

Provider::Provider(RegistryBase& registry)
 : _registry(registry)
{
}

Instance Provider::GetComponentInstance(std::type_index type)
{
  return GetComponentInstance(type, _providerDefaultTagIndex);
}

Instance Provider::GetComponentInstance(std::type_index type, std::type_index tag)
{
  Description& description = _registry.GetDescriptions(type).back();

  std::lock_guard<std::recursive_mutex> lock(_mutex);
  return ManageInstanceCreation(description, tag);
}

std::vector<Instance> Provider::GetComponentInstances(std::type_index type)
{
  return GetComponentInstances(type, _providerDefaultTagIndex);
}

std::vector<Instance> Provider::GetComponentInstances(std::type_index type, std::type_index tag)
{
  auto& descriptions = _registry.GetDescriptions(type);
  std::vector<Instance> result;
  result.reserve(descriptions.size());

  std::lock_guard<std::recursive_mutex> lock(_mutex);
  for (auto& description : descriptions)
  {
    result.push_back(ManageInstanceCreation(description, tag));
  }

  return result;
}

Instance Provider::ManageInstanceCreation(Description& description, std::type_index tag)
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
        auto key = std::make_pair(tag, description.GetCreator());
        auto it = _instances.find(key);
        if (it != _instances.end())
        {
          Instance ci = it->second;
          ci.instance = description.GetConverter().Convert(ci.instance);
          return ci;
        }
        Instance ci = description.GetCreator()->CreateInstance(*this);
        _instances[key] = ci;
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
  _registry.Register(Lifetime::Scoped, typeid(Provider), typeid(Provider),
                     detail::GetAdaptedObjectCreator(this, false),
                     detail::GetConverter<ScopedProvider, Provider>(), false);
}

Instance ScopedProvider::ManageInstanceCreation(Description& description, std::type_index tag)
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
        auto key = std::make_pair(tag, description.GetCreator());
        auto it = _instances.find(key);
        if (it != _instances.end())
        {
          Instance ci = it->second;
          ci.instance = description.GetConverter().Convert(ci.instance);
          return ci;
        }
        Instance ci = description.GetCreator()->CreateInstance(*this);
        _instances[key] = ci;
        ci.instance = description.GetConverter().Convert(ci.instance);
        return ci;
      }
    case Lifetime::Singleton:
      {
        return _parent.ManageInstanceCreation(description, tag);
      }
  }
}

