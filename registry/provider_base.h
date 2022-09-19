#pragma once

#include "instance.h"

#include <memory>
#include <typeindex>

class Creator;
using CreatorPtr = std::shared_ptr<Creator>;

class Description;
class RegistryBase;

namespace detail {

template<typename T> struct ComponentInstanceGetter;

} // namespace detail

class ProviderBase {
  struct ProviderBaseState;

public:
  ProviderBase(RegistryBase& registry);
  virtual ~ProviderBase();

protected:
  template<typename T> friend struct detail::ComponentInstanceGetter;

  void AddFloatingInstance(Instance instance);
  Instance GetOrCreateInstance(std::type_index tag, Description& description);

  Instance GetComponentInstance(std::type_index type);
  Instance GetComponentInstance(std::type_index type, std::type_index tag);

  std::vector<Instance> GetComponentInstances(std::type_index type);
  std::vector<Instance> GetComponentInstances(std::type_index type, std::type_index tag);

  virtual Instance ManageInstanceCreation(Description& description, std::type_index tag) = 0;

  RegistryBase& _registry;
  std::unique_ptr<ProviderBaseState> _state;
};
