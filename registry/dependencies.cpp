#include "dependencies.h"

namespace ac::registry {

bool AddDependencies(std::vector<AnyPtr>& dependencies, Instance& instance)
{
  dependencies.push_back(instance.instance);
  dependencies.push_back(instance.real_instance);
  dependencies.insert(dependencies.end(), instance.dependencies.begin(), instance.dependencies.end());
  return true;
}

bool AddDependencies(std::vector<AnyPtr>& dependencies, std::vector<Instance>& instances)
{
  for (Instance& instance : instances) {
    if (!AddDependencies(dependencies, instance)) {
      return false;
    }
  }
  return true;
}

} // namespace ac::registry
