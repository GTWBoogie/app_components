#pragma once

#include "instance.h"

namespace ac::registry {

bool AddDependencies(std::vector<AnyPtr>& dependencies, Instance& instance);
bool AddDependencies(std::vector<AnyPtr>& dependencies, std::vector<Instance>& instances);

} // namespace ac::registry
