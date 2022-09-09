#include "converter.h"

Converter::Converter(ConvertFunction converter)
 : _converter(converter)
{
}

AnyPtr Converter::Convert(AnyPtr instance)
{
  return _converter(instance);
}

