#include "provider.h"

#include "converters.h"
#include "creators.h"
#include "description.h"
#include "registry_base.h"

Provider::Provider(RegistryBase& registry)
 : ProviderBase(registry)
{
}

Instance Provider::ManageInstanceCreation(Description& description, std::type_index tag)
{
  switch (description.GetLifetime())
  {
    case Lifetime::Transient:
      {
        Instance ci = description.GetCreator()->CreateInstance(*this);
        AddFloatingInstance(ci);
        ci.instance = description.GetConverter().Convert(ci.instance);
        return ci;
      }
    case Lifetime::Scoped:
    case Lifetime::Singleton:
      {
        return GetOrCreateInstance(tag, description);
      }
  }
}

ScopedProvider Provider::GetScope()
{
  return ScopedProvider(*this);
}

ScopedProvider::ScopedProvider(Provider& parent)
 : Provider(parent._registry)
 , _parent(parent)
{
  _registry.Register(Lifetime::Scoped, typeid(Provider), typeid(Provider),
                     detail::GetAdaptedObjectCreator(this, false),
                     detail::GetConverter<ScopedProvider, Provider>(), false);
}

Instance ScopedProvider::ManageInstanceCreation(Description& description, std::type_index tag)
{
  switch (description.GetLifetime())
  {
    case Lifetime::Transient:
      {
        Instance ci = description.GetCreator()->CreateInstance(*this);
        AddFloatingInstance(ci);
        ci.instance = description.GetConverter().Convert(ci.instance);
        return ci;
      }
    case Lifetime::Scoped:
      {
        return GetOrCreateInstance(tag, description);
      }
    case Lifetime::Singleton:
      {
        return _parent.ManageInstanceCreation(description, tag);
      }
  }
}

