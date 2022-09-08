#include "description.h"

Description::Description(Lifetime lifetime, ComponentCreatorPtr creator, Converter converter)
 : _lifetime(lifetime)
 , _creator(creator)
 , _converter(converter)
{
}

Lifetime Description::GetLifetime() const
{
  return _lifetime;
}

ComponentCreatorPtr Description::GetCreator()
{
  return _creator;
}

Converter  Description::GetConverter()
{
  return _converter;
}

