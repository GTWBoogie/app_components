#include "provider.h"
#include "registry.h"

#define BOOST_TEST_MODULE instance creation tests
#include <boost/test/included/unit_test.hpp>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"

struct DefaultInitializableBase {
};

struct DefaultInitializableStruct : public DefaultInitializableBase {
  DefaultInitializableStruct()  { instances += 1; }
  ~DefaultInitializableStruct() { instances -= 1; }
  static size_t instances;
};
size_t DefaultInitializableStruct::instances = 0;

struct CreatableBase {
};

struct CreatableStruct : public CreatableBase {
  explicit CreatableStruct(int i)  { instances += i; }
  ~CreatableStruct() { instances -= 1; }

  static CreatableStruct* Create() { return new CreatableStruct(1); }
  static size_t instances;
};
size_t CreatableStruct::instances = 0;

BOOST_AUTO_TEST_CASE(register_and_create_simple_struct_instance_unmanaged)
{
  BOOST_TEST(DefaultInitializableStruct::instances == 0);
  DefaultInitializableStruct bs;
  BOOST_TEST(DefaultInitializableStruct::instances == 1);

  {
    Registry registry;
    Provider provider(registry);
    registry.AddInstance<DefaultInitializableStruct, DefaultInitializableBase>(&bs, false);
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    auto &created = provider.GetInstance<DefaultInitializableStruct>();
  }

  BOOST_TEST(DefaultInitializableStruct::instances == 1);
}

BOOST_AUTO_TEST_CASE(register_and_create_simple_struct_instance_unmanaged_by_reference)
{
  BOOST_TEST(DefaultInitializableStruct::instances == 0);
  DefaultInitializableStruct bs;
  BOOST_TEST(DefaultInitializableStruct::instances == 1);

  {
    Registry registry;
    Provider provider(registry);
    registry.AddInstance<DefaultInitializableStruct, DefaultInitializableBase>(bs);
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    auto &created = provider.GetInstance<DefaultInitializableStruct>();
  }

  BOOST_TEST(DefaultInitializableStruct::instances == 1);
}

BOOST_AUTO_TEST_CASE(register_and_create_simple_struct_instance_managed)
{
  BOOST_TEST(DefaultInitializableStruct::instances == 0);
  auto* bs = new DefaultInitializableStruct();
  BOOST_TEST(DefaultInitializableStruct::instances == 1);

  {
    Registry registry;
    Provider provider(registry);
    registry.AddInstance<DefaultInitializableStruct, DefaultInitializableBase>(bs);
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    auto &created = provider.GetInstance<DefaultInitializableStruct>();
  }

  BOOST_TEST(DefaultInitializableStruct::instances == 0);
}

BOOST_AUTO_TEST_CASE(register_and_create_multiple_simple_struct_instances_unmanaged)
{
  BOOST_TEST(DefaultInitializableStruct::instances == 0);
  DefaultInitializableStruct bs1;
  BOOST_TEST(DefaultInitializableStruct::instances == 1);
  DefaultInitializableStruct bs2;
  BOOST_TEST(DefaultInitializableStruct::instances == 2);

  {
    Registry registry;
    Provider provider(registry);
    registry.AddInstance<DefaultInitializableStruct, DefaultInitializableBase>(&bs1, false);
    registry.AddInstance<DefaultInitializableStruct, DefaultInitializableBase>(&bs2, false);
    BOOST_TEST(DefaultInitializableStruct::instances == 2);
    auto &created = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 2);
    BOOST_TEST(&created == &bs2);
    auto multiple = provider.GetInstances<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 2);
    BOOST_TEST(multiple.size() == 2);
  }

  BOOST_TEST(DefaultInitializableStruct::instances == 2);
}

BOOST_AUTO_TEST_CASE(register_and_create_multiple_simple_struct_instances_unmanaged_by_reference)
{
  BOOST_TEST(DefaultInitializableStruct::instances == 0);
  DefaultInitializableStruct bs1;
  BOOST_TEST(DefaultInitializableStruct::instances == 1);
  DefaultInitializableStruct bs2;
  BOOST_TEST(DefaultInitializableStruct::instances == 2);

  {
    Registry registry;
    Provider provider(registry);
    registry.AddInstance<DefaultInitializableStruct, DefaultInitializableBase>(bs1);
    registry.AddInstance<DefaultInitializableStruct, DefaultInitializableBase>(bs2);
    BOOST_TEST(DefaultInitializableStruct::instances == 2);
    auto &created = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 2);
    BOOST_TEST(&created == &bs2);
    auto multiple = provider.GetInstances<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 2);
    BOOST_TEST(multiple.size() == 2);
  }

  BOOST_TEST(DefaultInitializableStruct::instances == 2);
}

BOOST_AUTO_TEST_CASE(register_and_create_multiple_simple_struct_instances_managed)
{
  BOOST_TEST(DefaultInitializableStruct::instances == 0);
  auto* bs1 = new DefaultInitializableStruct();
  BOOST_TEST(DefaultInitializableStruct::instances == 1);
  auto* bs2 = new DefaultInitializableStruct();
  BOOST_TEST(DefaultInitializableStruct::instances == 2);

  {
    Registry registry;
    Provider provider(registry);
    registry.AddInstance<DefaultInitializableStruct, DefaultInitializableBase>(bs1);
    registry.AddInstance<DefaultInitializableStruct, DefaultInitializableBase>(bs2);
    BOOST_TEST(DefaultInitializableStruct::instances == 2);
    auto &created = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 2);
    BOOST_TEST(&created == bs2);
    auto multiple = provider.GetInstances<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 2);
    BOOST_TEST(multiple.size() == 2);
  }

  BOOST_TEST(DefaultInitializableStruct::instances == 0);
}

BOOST_AUTO_TEST_CASE(try_register_and_create_multiple_simple_struct_instances_unmanaged)
{
  BOOST_TEST(DefaultInitializableStruct::instances == 0);
  DefaultInitializableStruct bs1;
  BOOST_TEST(DefaultInitializableStruct::instances == 1);
  DefaultInitializableStruct bs2;
  BOOST_TEST(DefaultInitializableStruct::instances == 2);

  {
    Registry registry;
    Provider provider(registry);
    registry.TryAddInstance<DefaultInitializableStruct, DefaultInitializableBase>(&bs1, false);
    registry.TryAddInstance<DefaultInitializableStruct, DefaultInitializableBase>(&bs2, false);
    BOOST_TEST(DefaultInitializableStruct::instances == 2);
    auto &created = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 2);
    BOOST_TEST(&created == &bs1);
    auto multiple = provider.GetInstances<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 2);
    BOOST_TEST(multiple.size() == 1);
  }

  BOOST_TEST(DefaultInitializableStruct::instances == 2);
}

BOOST_AUTO_TEST_CASE(try_register_and_create_multiple_simple_struct_instances_unmanaged_by_reference)
{
  BOOST_TEST(DefaultInitializableStruct::instances == 0);
  DefaultInitializableStruct bs1;
  BOOST_TEST(DefaultInitializableStruct::instances == 1);
  DefaultInitializableStruct bs2;
  BOOST_TEST(DefaultInitializableStruct::instances == 2);

  {
    Registry registry;
    Provider provider(registry);
    registry.TryAddInstance<DefaultInitializableStruct, DefaultInitializableBase>(bs1);
    registry.TryAddInstance<DefaultInitializableStruct, DefaultInitializableBase>(bs2);
    BOOST_TEST(DefaultInitializableStruct::instances == 2);
    auto &created = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 2);
    BOOST_TEST(&created == &bs1);
    auto multiple = provider.GetInstances<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 2);
    BOOST_TEST(multiple.size() == 1);
  }

  BOOST_TEST(DefaultInitializableStruct::instances == 2);
}

BOOST_AUTO_TEST_CASE(try_register_and_create_multiple_simple_struct_instances_managed)
{
  BOOST_TEST(DefaultInitializableStruct::instances == 0);
  auto* bs1 = new DefaultInitializableStruct();
  BOOST_TEST(DefaultInitializableStruct::instances == 1);
  auto* bs2 = new DefaultInitializableStruct();
  BOOST_TEST(DefaultInitializableStruct::instances == 2);

  {
    Registry registry;
    Provider provider(registry);
    registry.TryAddInstance<DefaultInitializableStruct, DefaultInitializableBase>(bs1);
    registry.TryAddInstance<DefaultInitializableStruct, DefaultInitializableBase>(bs2);
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    auto &created = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    BOOST_TEST(&created == bs1);
    auto multiple = provider.GetInstances<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    BOOST_TEST(multiple.size() == 1);
  }

  BOOST_TEST(DefaultInitializableStruct::instances == 0);
}

BOOST_AUTO_TEST_CASE(register_and_create_simple_struct_singleton)
{
  BOOST_TEST(DefaultInitializableStruct::instances == 0);

  {
    Registry registry;
    Provider provider(registry);
    registry.AddSingleton<DefaultInitializableStruct, DefaultInitializableBase>();
    BOOST_TEST(DefaultInitializableStruct::instances == 0);
    auto &created1 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    auto &created2 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    BOOST_TEST(&created1 == &created2);
    auto multiple = provider.GetInstances<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    BOOST_TEST(multiple.size() == 1);
  }

  BOOST_TEST(DefaultInitializableStruct::instances == 0);
}

BOOST_AUTO_TEST_CASE(register_multiple_and_create_simple_struct_singleton)
{
  BOOST_TEST(DefaultInitializableStruct::instances == 0);

  {
    Registry registry;
    Provider provider(registry);
    registry.AddSingleton<DefaultInitializableStruct, DefaultInitializableBase>();
    BOOST_TEST(DefaultInitializableStruct::instances == 0);
    registry.AddSingleton<DefaultInitializableStruct, DefaultInitializableBase>();
    BOOST_TEST(DefaultInitializableStruct::instances == 0);
    auto &created1 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    auto &created2 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    BOOST_TEST(&created1 == &created2);
    auto multiple = provider.GetInstances<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 2);
    BOOST_TEST(multiple.size() == 2);
    BOOST_TEST(&multiple[0].get() != &multiple[1].get());
  }

  BOOST_TEST(DefaultInitializableStruct::instances == 0);
}

BOOST_AUTO_TEST_CASE(try_register_multiple_and_create_simple_struct_singleton)
{
  BOOST_TEST(DefaultInitializableStruct::instances == 0);

  {
    Registry registry;
    Provider provider(registry);
    registry.TryAddSingleton<DefaultInitializableStruct, DefaultInitializableBase>();
    BOOST_TEST(DefaultInitializableStruct::instances == 0);
    registry.TryAddSingleton<DefaultInitializableStruct, DefaultInitializableBase>();
    BOOST_TEST(DefaultInitializableStruct::instances == 0);
    auto &created1 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    auto &created2 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    BOOST_TEST(&created1 == &created2);
    auto multiple = provider.GetInstances<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    BOOST_TEST(multiple.size() == 1);
    BOOST_TEST(&multiple[0].get() == &created1);
  }

  BOOST_TEST(DefaultInitializableStruct::instances == 0);
}

BOOST_AUTO_TEST_CASE(register_multiple_and_create_simple_struct_singleton_factory)
{
  BOOST_TEST(DefaultInitializableStruct::instances == 0);

  {
    Registry registry;
    Provider provider(registry);
    registry.AddSingleton<DefaultInitializableStruct, DefaultInitializableBase>([]{ return new DefaultInitializableStruct; });
    BOOST_TEST(DefaultInitializableStruct::instances == 0);
    registry.AddSingleton<DefaultInitializableStruct, DefaultInitializableBase>([]{ return new DefaultInitializableStruct; });
    BOOST_TEST(DefaultInitializableStruct::instances == 0);
    auto &created1 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    auto &created2 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    BOOST_TEST(&created1 == &created2);
    auto multiple = provider.GetInstances<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 2);
    BOOST_TEST(multiple.size() == 2);
    BOOST_TEST(&multiple[0].get() != &multiple[1].get());
  }

  BOOST_TEST(DefaultInitializableStruct::instances == 0);
}

BOOST_AUTO_TEST_CASE(try_register_multiple_and_create_simple_struct_singleton_factory)
{
  BOOST_TEST(DefaultInitializableStruct::instances == 0);

  {
    Registry registry;
    Provider provider(registry);
    registry.TryAddSingleton<DefaultInitializableStruct, DefaultInitializableBase>([]{ return new DefaultInitializableStruct; });
    BOOST_TEST(DefaultInitializableStruct::instances == 0);
    registry.TryAddSingleton<DefaultInitializableStruct, DefaultInitializableBase>([]{ return new DefaultInitializableStruct; });
    BOOST_TEST(DefaultInitializableStruct::instances == 0);
    auto &created1 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    auto &created2 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    BOOST_TEST(&created1 == &created2);
    auto multiple = provider.GetInstances<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    BOOST_TEST(multiple.size() == 1);
    BOOST_TEST(&multiple[0].get() == &created1);
  }

  BOOST_TEST(DefaultInitializableStruct::instances == 0);
}

BOOST_AUTO_TEST_CASE(register_multiple_and_create_creatable_struct_singleton)
{
  BOOST_TEST(CreatableStruct::instances == 0);

  {
    Registry registry;
    Provider provider(registry);
    registry.AddSingleton<CreatableStruct, CreatableBase>();
    BOOST_TEST(CreatableStruct::instances == 0);
    registry.AddSingleton<CreatableStruct, CreatableBase>();
    BOOST_TEST(CreatableStruct::instances == 0);
    auto &created1 = provider.GetInstance<CreatableStruct>();
    BOOST_TEST(CreatableStruct::instances == 1);
    auto &created2 = provider.GetInstance<CreatableStruct>();
    BOOST_TEST(CreatableStruct::instances == 1);
    BOOST_TEST(&created1 == &created2);
    auto multiple = provider.GetInstances<CreatableStruct>();
    BOOST_TEST(CreatableStruct::instances == 2);
    BOOST_TEST(multiple.size() == 2);
    BOOST_TEST(&multiple[0].get() != &multiple[1].get());
  }

  BOOST_TEST(CreatableStruct::instances == 0);
}

BOOST_AUTO_TEST_CASE(try_register_multiple_and_create_creatable_struct_singleton)
{
  BOOST_TEST(CreatableStruct::instances == 0);

  {
    Registry registry;
    Provider provider(registry);
    registry.TryAddSingleton<CreatableStruct, CreatableBase>();
    BOOST_TEST(CreatableStruct::instances == 0);
    registry.TryAddSingleton<CreatableStruct, CreatableBase>();
    BOOST_TEST(CreatableStruct::instances == 0);
    auto &created1 = provider.GetInstance<CreatableStruct>();
    BOOST_TEST(CreatableStruct::instances == 1);
    auto &created2 = provider.GetInstance<CreatableStruct>();
    BOOST_TEST(CreatableStruct::instances == 1);
    BOOST_TEST(&created1 == &created2);
    auto multiple = provider.GetInstances<CreatableStruct>();
    BOOST_TEST(CreatableStruct::instances == 1);
    BOOST_TEST(multiple.size() == 1);
    BOOST_TEST(&multiple[0].get() == &created1);
  }

  BOOST_TEST(CreatableStruct::instances == 0);
}

BOOST_AUTO_TEST_CASE(register_and_create_simple_struct_scoped)
{
  BOOST_TEST(DefaultInitializableStruct::instances == 0);

  {
    Registry registry;
    Provider provider(registry);
    registry.AddScoped<DefaultInitializableStruct, DefaultInitializableBase>();
    BOOST_TEST(DefaultInitializableStruct::instances == 0);
    auto &created1 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    auto &created2 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    BOOST_TEST(&created1 == &created2);
    auto multiple = provider.GetInstances<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    BOOST_TEST(multiple.size() == 1);
  }

  BOOST_TEST(DefaultInitializableStruct::instances == 0);
}

BOOST_AUTO_TEST_CASE(register_multiple_and_create_simple_struct_scoped)
{
  BOOST_TEST(DefaultInitializableStruct::instances == 0);

  {
    Registry registry;
    Provider provider(registry);
    registry.AddScoped<DefaultInitializableStruct, DefaultInitializableBase>();
    BOOST_TEST(DefaultInitializableStruct::instances == 0);
    registry.AddScoped<DefaultInitializableStruct, DefaultInitializableBase>();
    BOOST_TEST(DefaultInitializableStruct::instances == 0);
    auto &created1 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    auto &created2 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    BOOST_TEST(&created1 == &created2);
    auto multiple = provider.GetInstances<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 2);
    BOOST_TEST(multiple.size() == 2);
    BOOST_TEST(&multiple[0].get() != &multiple[1].get());
  }

  BOOST_TEST(DefaultInitializableStruct::instances == 0);
}

BOOST_AUTO_TEST_CASE(try_register_multiple_and_create_simple_struct_scoped)
{
  BOOST_TEST(DefaultInitializableStruct::instances == 0);

  {
    Registry registry;
    Provider provider(registry);
    registry.TryAddScoped<DefaultInitializableStruct, DefaultInitializableBase>();
    BOOST_TEST(DefaultInitializableStruct::instances == 0);
    registry.TryAddScoped<DefaultInitializableStruct, DefaultInitializableBase>();
    BOOST_TEST(DefaultInitializableStruct::instances == 0);
    auto &created1 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    auto &created2 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    BOOST_TEST(&created1 == &created2);
    auto multiple = provider.GetInstances<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    BOOST_TEST(multiple.size() == 1);
    BOOST_TEST(&multiple[0].get() == &created1);
  }

  BOOST_TEST(DefaultInitializableStruct::instances == 0);
}

BOOST_AUTO_TEST_CASE(register_multiple_and_create_simple_struct_scoped_factory)
{
  BOOST_TEST(DefaultInitializableStruct::instances == 0);

  {
    Registry registry;
    Provider provider(registry);
    registry.AddScoped<DefaultInitializableStruct, DefaultInitializableBase>([]{ return new DefaultInitializableStruct; });
    BOOST_TEST(DefaultInitializableStruct::instances == 0);
    registry.AddScoped<DefaultInitializableStruct, DefaultInitializableBase>([]{ return new DefaultInitializableStruct; });
    BOOST_TEST(DefaultInitializableStruct::instances == 0);
    auto &created1 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    auto &created2 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    BOOST_TEST(&created1 == &created2);
    auto multiple = provider.GetInstances<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 2);
    BOOST_TEST(multiple.size() == 2);
    BOOST_TEST(&multiple[0].get() != &multiple[1].get());
  }

  BOOST_TEST(DefaultInitializableStruct::instances == 0);
}

BOOST_AUTO_TEST_CASE(try_register_multiple_and_create_simple_struct_scoped_factory)
{
  BOOST_TEST(DefaultInitializableStruct::instances == 0);

  {
    Registry registry;
    Provider provider(registry);
    registry.TryAddScoped<DefaultInitializableStruct, DefaultInitializableBase>([]{ return new DefaultInitializableStruct; });
    BOOST_TEST(DefaultInitializableStruct::instances == 0);
    registry.TryAddScoped<DefaultInitializableStruct, DefaultInitializableBase>([]{ return new DefaultInitializableStruct; });
    BOOST_TEST(DefaultInitializableStruct::instances == 0);
    auto &created1 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    auto &created2 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    BOOST_TEST(&created1 == &created2);
    auto multiple = provider.GetInstances<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    BOOST_TEST(multiple.size() == 1);
    BOOST_TEST(&multiple[0].get() == &created1);
  }

  BOOST_TEST(DefaultInitializableStruct::instances == 0);
}

BOOST_AUTO_TEST_CASE(register_multiple_and_create_creatable_struct_scoped)
{
  BOOST_TEST(CreatableStruct::instances == 0);

  {
    Registry registry;
    Provider provider(registry);
    registry.AddScoped<CreatableStruct, CreatableBase>();
    BOOST_TEST(CreatableStruct::instances == 0);
    registry.AddScoped<CreatableStruct, CreatableBase>();
    BOOST_TEST(CreatableStruct::instances == 0);
    auto &created1 = provider.GetInstance<CreatableStruct>();
    BOOST_TEST(CreatableStruct::instances == 1);
    auto &created2 = provider.GetInstance<CreatableStruct>();
    BOOST_TEST(CreatableStruct::instances == 1);
    BOOST_TEST(&created1 == &created2);
    auto multiple = provider.GetInstances<CreatableStruct>();
    BOOST_TEST(CreatableStruct::instances == 2);
    BOOST_TEST(multiple.size() == 2);
    BOOST_TEST(&multiple[0].get() != &multiple[1].get());
  }

  BOOST_TEST(CreatableStruct::instances == 0);
}

BOOST_AUTO_TEST_CASE(try_register_multiple_and_create_creatable_struct_scoped)
{
  BOOST_TEST(CreatableStruct::instances == 0);

  {
    Registry registry;
    Provider provider(registry);
    registry.TryAddScoped<CreatableStruct, CreatableBase>();
    BOOST_TEST(CreatableStruct::instances == 0);
    registry.TryAddScoped<CreatableStruct, CreatableBase>();
    BOOST_TEST(CreatableStruct::instances == 0);
    auto &created1 = provider.GetInstance<CreatableStruct>();
    BOOST_TEST(CreatableStruct::instances == 1);
    auto &created2 = provider.GetInstance<CreatableStruct>();
    BOOST_TEST(CreatableStruct::instances == 1);
    BOOST_TEST(&created1 == &created2);
    auto multiple = provider.GetInstances<CreatableStruct>();
    BOOST_TEST(CreatableStruct::instances == 1);
    BOOST_TEST(multiple.size() == 1);
    BOOST_TEST(&multiple[0].get() == &created1);
  }

  BOOST_TEST(CreatableStruct::instances == 0);
}

BOOST_AUTO_TEST_CASE(register_and_create_simple_struct_transient)
{
  BOOST_TEST(DefaultInitializableStruct::instances == 0);

  {
    Registry registry;
    Provider provider(registry);
    registry.AddTransient<DefaultInitializableStruct, DefaultInitializableBase>();
    BOOST_TEST(DefaultInitializableStruct::instances == 0);
    auto &created1 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    auto &created2 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 2);
    BOOST_TEST(&created1 != &created2);
    auto multiple = provider.GetInstances<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 3);
    BOOST_TEST(multiple.size() == 1);
  }

  BOOST_TEST(DefaultInitializableStruct::instances == 0);
}

BOOST_AUTO_TEST_CASE(register_multiple_and_create_simple_struct_transient)
{
  BOOST_TEST(DefaultInitializableStruct::instances == 0);

  {
    Registry registry;
    Provider provider(registry);
    registry.AddTransient<DefaultInitializableStruct, DefaultInitializableBase>();
    BOOST_TEST(DefaultInitializableStruct::instances == 0);
    registry.AddTransient<DefaultInitializableStruct, DefaultInitializableBase>();
    BOOST_TEST(DefaultInitializableStruct::instances == 0);
    auto &created1 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    auto &created2 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 2);
    BOOST_TEST(&created1 != &created2);
    auto multiple = provider.GetInstances<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 4);
    BOOST_TEST(multiple.size() == 2);
    BOOST_TEST(&multiple[0].get() != &created1);
    BOOST_TEST(&multiple[0].get() != &created2);
    BOOST_TEST(&multiple[1].get() != &created1);
    BOOST_TEST(&multiple[1].get() != &created2);
    BOOST_TEST(&multiple[0].get() != &multiple[1].get());
  }

  BOOST_TEST(DefaultInitializableStruct::instances == 0);
}

BOOST_AUTO_TEST_CASE(try_register_multiple_and_create_simple_struct_transient)
{
  BOOST_TEST(DefaultInitializableStruct::instances == 0);

  {
    Registry registry;
    Provider provider(registry);
    registry.TryAddTransient<DefaultInitializableStruct, DefaultInitializableBase>();
    BOOST_TEST(DefaultInitializableStruct::instances == 0);
    registry.TryAddTransient<DefaultInitializableStruct, DefaultInitializableBase>();
    BOOST_TEST(DefaultInitializableStruct::instances == 0);
    auto &created1 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    auto &created2 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 2);
    BOOST_TEST(&created1 != &created2);
    auto multiple = provider.GetInstances<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 3);
    BOOST_TEST(multiple.size() == 1);
    BOOST_TEST(&multiple[0].get() != &created1);
    BOOST_TEST(&multiple[0].get() != &created2);
  }

  BOOST_TEST(DefaultInitializableStruct::instances == 0);
}

BOOST_AUTO_TEST_CASE(register_multiple_and_create_simple_struct_transient_factory)
{
  BOOST_TEST(DefaultInitializableStruct::instances == 0);

  {
    Registry registry;
    Provider provider(registry);
    registry.AddTransient<DefaultInitializableStruct, DefaultInitializableBase>([]{ return new DefaultInitializableStruct; });
    BOOST_TEST(DefaultInitializableStruct::instances == 0);
    registry.AddTransient<DefaultInitializableStruct, DefaultInitializableBase>([]{ return new DefaultInitializableStruct; });
    BOOST_TEST(DefaultInitializableStruct::instances == 0);
    auto &created1 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    auto &created2 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 2);
    BOOST_TEST(&created1 != &created2);
    auto multiple = provider.GetInstances<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 4);
    BOOST_TEST(multiple.size() == 2);
    BOOST_TEST(&multiple[0].get() != &created1);
    BOOST_TEST(&multiple[0].get() != &created2);
    BOOST_TEST(&multiple[1].get() != &created1);
    BOOST_TEST(&multiple[1].get() != &created2);
    BOOST_TEST(&multiple[0].get() != &multiple[1].get());
  }

  BOOST_TEST(DefaultInitializableStruct::instances == 0);
}

BOOST_AUTO_TEST_CASE(try_register_multiple_and_create_simple_struct_transient_factory)
{
  BOOST_TEST(DefaultInitializableStruct::instances == 0);

  {
    Registry registry;
    Provider provider(registry);
    registry.TryAddTransient<DefaultInitializableStruct, DefaultInitializableBase>([]{ return new DefaultInitializableStruct; });
    BOOST_TEST(DefaultInitializableStruct::instances == 0);
    registry.TryAddTransient<DefaultInitializableStruct, DefaultInitializableBase>([]{ return new DefaultInitializableStruct; });
    BOOST_TEST(DefaultInitializableStruct::instances == 0);
    auto &created1 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 1);
    auto &created2 = provider.GetInstance<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 2);
    BOOST_TEST(&created1 != &created2);
    auto multiple = provider.GetInstances<DefaultInitializableStruct>();
    BOOST_TEST(DefaultInitializableStruct::instances == 3);
    BOOST_TEST(multiple.size() == 1);
    BOOST_TEST(&multiple[0].get() != &created1);
    BOOST_TEST(&multiple[0].get() != &created2);
  }

  BOOST_TEST(DefaultInitializableStruct::instances == 0);
}

BOOST_AUTO_TEST_CASE(register_multiple_and_create_creatable_struct_transient)
{
  BOOST_TEST(CreatableStruct::instances == 0);

  {
    Registry registry;
    Provider provider(registry);
    registry.AddTransient<CreatableStruct, CreatableBase>();
    BOOST_TEST(CreatableStruct::instances == 0);
    registry.AddTransient<CreatableStruct, CreatableBase>();
    BOOST_TEST(CreatableStruct::instances == 0);
    auto &created1 = provider.GetInstance<CreatableStruct>();
    BOOST_TEST(CreatableStruct::instances == 1);
    auto &created2 = provider.GetInstance<CreatableStruct>();
    BOOST_TEST(CreatableStruct::instances == 2);
    BOOST_TEST(&created1 != &created2);
    auto multiple = provider.GetInstances<CreatableStruct>();
    BOOST_TEST(CreatableStruct::instances == 4);
    BOOST_TEST(multiple.size() == 2);
    BOOST_TEST(&multiple[0].get() != &created1);
    BOOST_TEST(&multiple[0].get() != &created2);
    BOOST_TEST(&multiple[1].get() != &created1);
    BOOST_TEST(&multiple[1].get() != &created2);
    BOOST_TEST(&multiple[0].get() != &multiple[1].get());
  }

  BOOST_TEST(CreatableStruct::instances == 0);
}

BOOST_AUTO_TEST_CASE(try_register_multiple_and_create_creatable_struct_transient)
{
  BOOST_TEST(CreatableStruct::instances == 0);

  {
    Registry registry;
    Provider provider(registry);
    registry.TryAddTransient<CreatableStruct, CreatableBase>();
    BOOST_TEST(CreatableStruct::instances == 0);
    registry.TryAddTransient<CreatableStruct, CreatableBase>();
    BOOST_TEST(CreatableStruct::instances == 0);
    auto &created1 = provider.GetInstance<CreatableStruct>();
    BOOST_TEST(CreatableStruct::instances == 1);
    auto &created2 = provider.GetInstance<CreatableStruct>();
    BOOST_TEST(CreatableStruct::instances == 2);
    BOOST_TEST(&created1 != &created2);
    auto multiple = provider.GetInstances<CreatableStruct>();
    BOOST_TEST(CreatableStruct::instances == 3);
    BOOST_TEST(multiple.size() == 1);
    BOOST_TEST(&multiple[0].get() != &created1);
    BOOST_TEST(&multiple[0].get() != &created2);
  }

  BOOST_TEST(CreatableStruct::instances == 0);
}

#pragma clang diagnostic pop
