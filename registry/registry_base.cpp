#include "registry_base.h"

#include "util/debug_log.h"
#include "util/demangle.h"

void RegistryBase::Register(Lifetime lifetime, std::type_index type_index, std::type_index class_index,
                            CreatorPtr creator, Converter converter, bool unique)
{
  auto it = _descriptions.find(type_index);
  if (unique && it != _descriptions.end())
  {
    DLOG("Interface " << util::get_demangled_type_name(type_index.name())
                      << " already registered - skipping registering for class " << util::get_demangled_type_name(class_index.name()));
  }
  else
  {
    _descriptions[type_index].push_back(Description(lifetime, creator, converter));
    DLOG("Registered interface " << util::get_demangled_type_name(type_index.name())
                                 << " for class " << util::get_demangled_type_name(class_index.name()));
  }
}

std::vector<Description>& RegistryBase::GetDescriptions(std::type_index type)
{
  auto it = _descriptions.find(type);
  if (it == _descriptions.end())
  {
    throw std::invalid_argument(std::string("Object providing interface ") + util::get_demangled_type_name(type.name()) + " not registered");
  }

  return it->second;
}