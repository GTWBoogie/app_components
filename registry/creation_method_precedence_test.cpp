#include "provider.h"
#include "registry.h"

#define BOOST_TEST_MODULE creation method precedence tests
#include <boost/test/included/unit_test.hpp>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"

struct DefaulInitializableAndCreatableStruct {
  explicit DefaulInitializableAndCreatableStruct()
          : created_by_create(false)
  {}

  static DefaulInitializableAndCreatableStruct* Create()
  {
    DefaulInitializableAndCreatableStruct *result = new DefaulInitializableAndCreatableStruct();
    result->created_by_create = true;
    return result;
  }

  bool created_by_create;
};

BOOST_AUTO_TEST_CASE(CreatableTakesPrecedenseOverDefaultInitializable)
{
  ac::registry::Registry registry;
  ac::registry::Provider provider(registry);

  registry.AddSingleton<DefaulInitializableAndCreatableStruct>();
  auto &created = provider.GetInstance<DefaulInitializableAndCreatableStruct>();
  BOOST_TEST(created.created_by_create == true);
}

struct ConstructorExposedAndCreatableStruct {
  CONSTRUCTOR(ConstructorExposedAndCreatableStruct(int))
          : created_by_create(false)
  {}

  static ConstructorExposedAndCreatableStruct* Create(int i)
  {
    ConstructorExposedAndCreatableStruct *result = new ConstructorExposedAndCreatableStruct(i);
    result->created_by_create = true;
    return result;
  }

  bool created_by_create;
};

BOOST_AUTO_TEST_CASE(CreatableTakesPrecedenseOverConstructorExposed)
{
  ac::registry::Registry registry;
  ac::registry::Provider provider(registry);
  int i = 0; registry.AddInstance(i);

  registry.AddSingleton<ConstructorExposedAndCreatableStruct>();
  auto &created = provider.GetInstance<ConstructorExposedAndCreatableStruct>();
  BOOST_TEST(created.created_by_create == true);
}

#pragma clang diagnostic pop
