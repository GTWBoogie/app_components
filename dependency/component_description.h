#pragma once

#include "lifetime.h"
#include "component_creator.h"
#include "component_converter.h"

class ComponentDescription {
public:
  ComponentDescription(Lifetime lifetime, ComponentCreatorPtr creator, ComponentConverter converter);

  Lifetime GetLifetime() const; 
  ComponentCreatorPtr GetCreator();
  ComponentConverter  GetConverter();

private:
  Lifetime _lifetime;
  ComponentCreatorPtr _creator;
  ComponentConverter  _converter;
};
