#include "description.h"

Description::Description(Lifetime lifetime, CreatorPtr creator, Converter converter)
 : _lifetime(lifetime)
 , _creator(creator)
 , _converter(converter)
{
}

Lifetime Description::GetLifetime() const
{
  return _lifetime;
}

CreatorPtr Description::GetCreator()
{
  return _creator;
}

Converter  Description::GetConverter()
{
  return _converter;
}

