#include "component_description.h"

ComponentDescription::ComponentDescription(Lifetime lifetime, ComponentCreatorPtr creator, ComponentConverter converter)
 : _lifetime(lifetime)
 , _creator(creator)
 , _converter(converter)
{
}

Lifetime ComponentDescription::GetLifetime() const
{
  return _lifetime;
}

ComponentCreatorPtr ComponentDescription::GetCreator()
{
  return _creator;
}

ComponentConverter  ComponentDescription::GetConverter()
{
  return _converter;
}

