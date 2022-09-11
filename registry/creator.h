#pragma once

#include "instance.h"

#include <functional>
#include <memory>
#include <utility>

class Provider;

class Creator;
using CreatorPtr = std::shared_ptr<Creator>;

class Creator : std::enable_shared_from_this<Creator>
{
public:
  using CreateFunction = std::function<Instance(Provider&)>;
  
  [[nodiscard]] static CreatorPtr Create(CreateFunction creator) { return CreatorPtr(new Creator(std::move(creator))); }

  CreatorPtr GetPointer() { return shared_from_this(); }

  Instance CreateInstance(Provider& registry) { return _creator(registry); }

private:
  explicit Creator(CreateFunction creator)
   : _creator(std::move(creator))
  {
  }

  CreateFunction _creator;
};
