#pragma once

#include <any>
#include <functional>
#include <memory>

using AnyPtr = std::shared_ptr<std::any>;

class ComponentConverter
{
public:
  using ConvertFunction = std::function<AnyPtr(AnyPtr)>;

  ComponentConverter(ConvertFunction converter);

  AnyPtr Convert(AnyPtr instance);

private:
  ConvertFunction _converter;
};

