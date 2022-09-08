#pragma once

#include "instance.h"

#include <functional>
#include <memory>

class Provider;

class Creator;
using ComponentCreatorPtr = std::shared_ptr<Creator>;

class Creator : std::enable_shared_from_this<Creator>
{
public:
  using CreateFunction = std::function<Instance(Provider&)>;
  
  [[nodiscard]] static ComponentCreatorPtr Create(CreateFunction creator);

  ComponentCreatorPtr GetPointer();

  Instance CreateInstance(Provider& registry);

private:
  Creator(CreateFunction creator);

  CreateFunction _creator;
};

