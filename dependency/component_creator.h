#pragma once

#include "component_instance.h"

#include <functional>
#include <memory>

class ComponentProvider;

class ComponentCreator;
using ComponentCreatorPtr = std::shared_ptr<ComponentCreator>;

class ComponentCreator : std::enable_shared_from_this<ComponentCreator>
{
public:
  using CreateFunction = std::function<ComponentInstance(ComponentProvider&)>;
  
  [[nodiscard]] static ComponentCreatorPtr Create(CreateFunction creator);

  ComponentCreatorPtr GetPointer();

  ComponentInstance CreateInstance(ComponentProvider& registry);

private:
  ComponentCreator(CreateFunction creator);

  CreateFunction _creator;
};

