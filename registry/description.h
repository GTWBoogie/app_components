#pragma once

#include <utility>

#include "converter.h"
#include "lifetime.h"

class Creator;
using CreatorPtr = std::shared_ptr<Creator>;

class Description {
public:
  Description(Lifetime lifetime, CreatorPtr creator, Converter converter)
   : _lifetime(lifetime)
  , _creator(std::move(creator))
  , _converter(std::move(converter))
  {
  }

  [[nodiscard]] Lifetime GetLifetime() const { return _lifetime; }
  [[nodiscard]] CreatorPtr GetCreator() const { return _creator; }
  [[nodiscard]] Converter  GetConverter() const { return _converter; }

private:
  Lifetime _lifetime;
  CreatorPtr _creator;
  Converter  _converter;
};
