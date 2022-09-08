#pragma once

#include "creator.h"
#include "converter.h"

class Description {
public:
  Description(Lifetime lifetime, ComponentCreatorPtr creator, Converter converter);

  Lifetime GetLifetime() const; 
  ComponentCreatorPtr GetCreator();
  Converter  GetConverter();

private:
  Lifetime _lifetime;
  ComponentCreatorPtr _creator;
  Converter  _converter;
};
