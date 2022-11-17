#pragma once

#include "instance.h"

#include <functional>
#include <memory>

class ProviderBase;

class Creator;
using CreatorPtr = std::shared_ptr<Creator>;

class Creator
{
public:
  using CreateFunction = std::function<Instance(ProviderBase&)>;
  
  [[nodiscard]] static CreatorPtr Create(CreateFunction creator) { return CreatorPtr(new Creator(std::move(creator))); }

  Instance CreateInstance(ProviderBase& registry) { return _creator(registry); }

private:
  explicit Creator(CreateFunction creator)
   : _creator(std::move(creator))
  {
  }

  CreateFunction _creator;
};
