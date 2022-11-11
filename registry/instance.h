#pragma once

#include <any>
#include <memory>
#include <vector>

using AnyPtr = std::shared_ptr<std::any>;

struct Instance {
  ~Instance()
  {
    instance.reset();
    real_instance.reset();
    dependencies.clear();
  }

  AnyPtr instance;
  AnyPtr real_instance;
  std::vector<AnyPtr> dependencies;
};

