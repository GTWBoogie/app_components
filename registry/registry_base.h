#pragma once

#include "lifetime.h"

#include <memory>
#include <typeindex>
#include <vector>

class Creator;
using CreatorPtr = std::shared_ptr<Creator>;

class Converter;
class Description;
class ProviderBase;
class ScopedProvider;

class RegistryBase {
  struct RegistryBaseState;
public:
  RegistryBase();
  virtual ~RegistryBase();

protected:
  std::vector<Description>& GetDescriptions(std::type_index type);

  void Register(Lifetime lifetime, std::type_index type_index, std::type_index class_index,
                CreatorPtr creator, Converter converter, bool unique = false);

private:
  friend class ProviderBase;
  friend class ScopedProvider;

  std::unique_ptr<RegistryBaseState> _state;
};
