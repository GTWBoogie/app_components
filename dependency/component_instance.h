#pragma once

#include "lifetime.h"

#include <any>
#include <memory>
#include <vector>

using AnyPtr = std::shared_ptr<std::any>;

struct ComponentInstance {
  AnyPtr instance;
  std::vector<AnyPtr> dependencies;
};
