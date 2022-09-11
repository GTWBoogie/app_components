#pragma once

#include "instance.h"

#include <map>
#include <mutex>
#include <typeindex>
#include <vector>

class Description;
class Provider;
class Registry;
class ScopedProvider;

class Creator;
using CreatorPtr = std::shared_ptr<Creator>;

namespace detail {

template<typename T> T MakeArgumentsTuple(Provider&, std::vector<AnyPtr>&);

} // namespace detail

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
  friend class ScopedProvider;
  template<typename T> friend T detail::MakeArgumentsTuple(Provider&, std::vector<AnyPtr>&);

  Instance GetComponentInstance(std::type_index type);
  std::vector<Instance> GetComponentInstances(std::type_index type);

  virtual Instance ManageInstanceCreation(Description& description);

  std::recursive_mutex _mutex;
  Registry& _registry;
  std::map<CreatorPtr, Instance> _instances;
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

