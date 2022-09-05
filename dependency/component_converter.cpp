#include "component_converter.h"

ComponentConverter::ComponentConverter(ConvertFunction converter)
 : _converter(converter)
{
}

AnyPtr ComponentConverter::Convert(AnyPtr instance)
{
  return _converter(instance);
}

