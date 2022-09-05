#include "component_creator.h"

ComponentCreatorPtr ComponentCreator::Create(CreateFunction creator)
{
  return ComponentCreatorPtr(new ComponentCreator(creator));
}

ComponentCreatorPtr ComponentCreator::GetPointer()
{
  return shared_from_this();
}

ComponentInstance ComponentCreator::CreateInstance(ComponentProvider& registry)
{
  return _creator(registry);
}

ComponentCreator::ComponentCreator(CreateFunction creator)
 : _creator(creator)
{
}

