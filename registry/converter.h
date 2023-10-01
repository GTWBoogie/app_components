#pragma once

#include <any>
#include <functional>
#include <memory>
#include <utility>

namespace ac::registry {

using AnyPtr = std::shared_ptr<std::any>;

class Converter
{
public:
  using ConvertFunction = std::function<AnyPtr(AnyPtr)>;

  explicit Converter(ConvertFunction converter)
    : _converter(std::move(converter))
  {
  }

  AnyPtr Convert(AnyPtr instance)
  {
    return _converter(std::move(instance));
  }

private:
  ConvertFunction _converter;
};

} // namespace ac::registry
