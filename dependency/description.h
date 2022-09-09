#pragma once

#include "creator.h"
#include "converter.h"

class Description {
public:
  Description(Lifetime lifetime, CreatorPtr creator, Converter converter);

  Lifetime GetLifetime() const; 
  CreatorPtr GetCreator();
  Converter  GetConverter();

private:
  Lifetime _lifetime;
  CreatorPtr _creator;
  Converter  _converter;
};
