#pragma once

#include "description.h"

#include <map>
#include <typeindex>
#include <vector>

class RegistryBase {
public:
  RegistryBase() = default;
  virtual ~RegistryBase() = default;

protected:
  std::vector<Description>& GetDescriptions(std::type_index type);

  void Register(Lifetime lifetime, std::type_index type_index, std::type_index class_index,
                CreatorPtr creator, Converter converter, bool unique = false);

private:
  friend class Provider;
  friend class ScopedProvider;

  std::map<std::type_index, std::vector<Description>> _descriptions;
};
