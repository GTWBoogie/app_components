#include "creator.h"

ComponentCreatorPtr Creator::Create(CreateFunction creator)
{
  return ComponentCreatorPtr(new Creator(creator));
}

ComponentCreatorPtr Creator::GetPointer()
{
  return shared_from_this();
}

Instance Creator::CreateInstance(Provider& registry)
{
  return _creator(registry);
}

Creator::Creator(CreateFunction creator)
 : _creator(creator)
{
}

