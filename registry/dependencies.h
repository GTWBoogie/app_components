#pragma once

#include "instance.h"

bool AddDependencies(std::vector<AnyPtr>& dependencies, Instance& instance);
bool AddDependencies(std::vector<AnyPtr>& dependencies, std::vector<Instance>& instances);
