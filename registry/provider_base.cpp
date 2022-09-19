#include "provider_base.h"

#include "creator.h"
#include "description.h"
#include "registry_base.h"

#include <map>
#include <mutex>
#include <vector>

namespace {

struct ProviderDefaultTag{};
const std::type_index _providerDefaultTagIndex = std::type_index(typeid(ProviderDefaultTag));

} // namespace

struct ProviderBase::ProviderBaseState
{
  std::recursive_mutex _mutex;
  std::map<std::pair<std::type_index, CreatorPtr>, Instance> _instances;
  std::vector<Instance> _floating_instances;
};

ProviderBase::ProviderBase(RegistryBase& registry)
 : _registry(registry)
 , _state(std::make_unique<ProviderBaseState>())
{
}

ProviderBase::~ProviderBase() {}

void ProviderBase::AddFloatingInstance(Instance instance)
{
  _state->_floating_instances.push_back(instance);
}

Instance ProviderBase::GetOrCreateInstance(std::type_index tag, Description& description)
{
  auto key = std::make_pair(tag, description.GetCreator());
  auto it = _state->_instances.find(key);
  if (it != _state->_instances.end())
  {
    Instance ci = it->second;
    ci.instance = description.GetConverter().Convert(ci.instance);
    return ci;
  }
  Instance ci = description.GetCreator()->CreateInstance(*this);
  _state->_instances[key] = ci;
  ci.instance = description.GetConverter().Convert(ci.instance);
  return ci;
}

Instance ProviderBase::GetComponentInstance(std::type_index type)
{
  return GetComponentInstance(type, _providerDefaultTagIndex);
}

Instance ProviderBase::GetComponentInstance(std::type_index type, std::type_index tag)
{
  Description& description = _registry.GetDescriptions(type).back();

  std::lock_guard<std::recursive_mutex> lock(_state->_mutex);
  return ManageInstanceCreation(description, tag);
}

std::vector<Instance> ProviderBase::GetComponentInstances(std::type_index type)
{
  return GetComponentInstances(type, _providerDefaultTagIndex);
}

std::vector<Instance> ProviderBase::GetComponentInstances(std::type_index type, std::type_index tag)
{
  auto& descriptions = _registry.GetDescriptions(type);
  std::vector<Instance> result;
  result.reserve(descriptions.size());

  std::lock_guard<std::recursive_mutex> lock(_state->_mutex);
  for (auto& description : descriptions)
  {
    result.push_back(ManageInstanceCreation(description, tag));
  }

  return result;
}
