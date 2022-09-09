#include "creator.h"

CreatorPtr Creator::Create(CreateFunction creator)
{
  return CreatorPtr(new Creator(creator));
}

CreatorPtr Creator::GetPointer()
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

