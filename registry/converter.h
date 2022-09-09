#pragma once

#include <any>
#include <functional>
#include <memory>

using AnyPtr = std::shared_ptr<std::any>;

class Converter
{
public:
  using ConvertFunction = std::function<AnyPtr(AnyPtr)>;

  Converter(ConvertFunction converter);

  AnyPtr Convert(AnyPtr instance);

private:
  ConvertFunction _converter;
};

