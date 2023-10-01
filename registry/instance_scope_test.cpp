#include "provider.h"
#include "registry.h"

#define BOOST_TEST_MODULE instance scope tests
#include <boost/test/included/unit_test.hpp>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"

struct SimpleStruct {};

BOOST_AUTO_TEST_CASE(create_adapted_instance_in_scopes)
{
  ac::registry::Registry registry;
  ac::registry::Provider provider(registry);
  SimpleStruct instance1;
  registry.AddInstance<SimpleStruct>(&instance1, false);
  SimpleStruct instance2;

  auto &created1 = provider.GetInstance<SimpleStruct>();
  BOOST_TEST(&created1 == &instance1);
  auto &created2 = provider.GetInstance<SimpleStruct>();
  BOOST_TEST(&created2 == &instance1);
  {
    registry.AddInstance<SimpleStruct>(&instance2, false);

    auto scope = provider.GetScope();
    auto &created3 = scope.GetInstance<SimpleStruct>();
    BOOST_TEST(&created3 == &instance2);
    auto &created4 = scope.GetInstance<SimpleStruct>();
    BOOST_TEST(&created4 == &instance2);
  }
  {
    auto scope = provider.GetScope();
    auto &created5 = scope.GetInstance<SimpleStruct>();
    BOOST_TEST(&created5 == &instance2);
    auto &created6 = scope.GetInstance<SimpleStruct>();
    BOOST_TEST(&created6 == &instance2);
  }
}

BOOST_AUTO_TEST_CASE(create_singleton_instance_in_scopes)
{
  ac::registry::Registry registry;
  ac::registry::Provider provider(registry);
  registry.AddSingleton<SimpleStruct>();

  auto &created1 = provider.GetInstance<SimpleStruct>();
  auto &created2 = provider.GetInstance<SimpleStruct>();
  BOOST_TEST(&created1 == &created2);
  {
    auto scope = provider.GetScope();
    auto &created3 = scope.GetInstance<SimpleStruct>();
    BOOST_TEST(&created3 == &created1);
    auto &created4 = scope.GetInstance<SimpleStruct>();
    BOOST_TEST(&created4 == &created1);
  }
  {
    auto scope = provider.GetScope();
    auto &created5 = scope.GetInstance<SimpleStruct>();
    BOOST_TEST(&created5 == &created1);
    auto &created6 = scope.GetInstance<SimpleStruct>();
    BOOST_TEST(&created6 == &created1);
  }
}

BOOST_AUTO_TEST_CASE(create_scoped_instance_in_scopes)
{
  ac::registry::Registry registry;
  ac::registry::Provider provider(registry);
  registry.AddScoped<SimpleStruct>();

  auto &created1 = provider.GetInstance<SimpleStruct>();
  auto &created2 = provider.GetInstance<SimpleStruct>();
  BOOST_TEST(&created1 == &created2);
  {
    auto scope = provider.GetScope();
    auto &created3 = scope.GetInstance<SimpleStruct>();
    BOOST_TEST(&created3 != &created1);
    auto &created4 = scope.GetInstance<SimpleStruct>();
    BOOST_TEST(&created4 == &created3);
  }
  {
    auto scope = provider.GetScope();
    auto &created5 = scope.GetInstance<SimpleStruct>();
    BOOST_TEST(&created5 != &created1);
    auto &created6 = scope.GetInstance<SimpleStruct>();
    BOOST_TEST(&created6 == &created5);
  }
}

BOOST_AUTO_TEST_CASE(create_transient_instance_in_scopes)
{
  ac::registry::Registry registry;
  ac::registry::Provider provider(registry);
  registry.AddTransient<SimpleStruct>();

  auto &created1 = provider.GetInstance<SimpleStruct>();
  auto &created2 = provider.GetInstance<SimpleStruct>();
  BOOST_TEST(&created1 != &created2);
  {
    auto scope = provider.GetScope();
    auto &created3 = scope.GetInstance<SimpleStruct>();
    BOOST_TEST(&created3 != &created1);
    BOOST_TEST(&created3 != &created2);
    auto &created4 = scope.GetInstance<SimpleStruct>();
    BOOST_TEST(&created4 != &created3);
  }
  {
    auto scope = provider.GetScope();
    auto &created5 = scope.GetInstance<SimpleStruct>();
    BOOST_TEST(&created5 != &created1);
    BOOST_TEST(&created5 != &created2);
    auto &created6 = scope.GetInstance<SimpleStruct>();
    BOOST_TEST(&created6 != &created5);
  }
}

struct SimpleTag {};

BOOST_AUTO_TEST_CASE(create_tagged_adapted_instance_in_scopes)
{
  ac::registry::Registry registry;
  ac::registry::Provider provider(registry);
  SimpleStruct instance1;
  registry.AddInstance<SimpleStruct>(&instance1, false);
  SimpleStruct instance2;

  auto &created1 = provider.GetInstance<SimpleStruct>();
  BOOST_TEST(&created1 == &instance1);
  auto created2 = provider.GetInstance<ac::registry::Tagged<SimpleStruct, SimpleTag>>();
  BOOST_TEST(&created2.Value() == &instance1);
  {
    registry.AddInstance<SimpleStruct>(&instance2, false);

    auto scope = provider.GetScope();
    auto &created3 = scope.GetInstance<SimpleStruct>();
    BOOST_TEST(&created3 == &instance2);
    auto created4 = scope.GetInstance<ac::registry::Tagged<SimpleStruct, SimpleTag>>();
    BOOST_TEST(&created4.Value() == &instance2);
  }
  {
    auto scope = provider.GetScope();
    auto &created5 = scope.GetInstance<SimpleStruct>();
    BOOST_TEST(&created5 == &instance2);
    auto created6 = scope.GetInstance<ac::registry::Tagged<SimpleStruct, SimpleTag>>();
    BOOST_TEST(&created6.Value() == &instance2);
  }
}

BOOST_AUTO_TEST_CASE(create_tagged_singleton_instance_in_scopes)
{
  ac::registry::Registry registry;
  ac::registry::Provider provider(registry);
  registry.AddSingleton<SimpleStruct>();

  auto &created1 = provider.GetInstance<SimpleStruct>();
  auto created2 = provider.GetInstance<ac::registry::Tagged<SimpleStruct, SimpleTag>>();
  BOOST_TEST(&created2.Value() != &created1);
  {
    auto scope = provider.GetScope();
    auto &created3 = scope.GetInstance<SimpleStruct>();
    BOOST_TEST(&created3 == &created1);
    auto created4 = scope.GetInstance<ac::registry::Tagged<SimpleStruct, SimpleTag>>();
    BOOST_TEST(&created4.Value() == &created2.Value());
  }
  {
    auto scope = provider.GetScope();
    auto &created5 = scope.GetInstance<SimpleStruct>();
    BOOST_TEST(&created5 == &created1);
    auto created6 = scope.GetInstance<ac::registry::Tagged<SimpleStruct, SimpleTag>>();
    BOOST_TEST(&created6.Value() == &created2.Value());
  }
}

BOOST_AUTO_TEST_CASE(create_tagged_scoped_instance_in_scopes)
{
  ac::registry::Registry registry;
  ac::registry::Provider provider(registry);
  registry.AddScoped<SimpleStruct>();

  auto &created1 = provider.GetInstance<SimpleStruct>();
  auto created2 = provider.GetInstance<ac::registry::Tagged<SimpleStruct, SimpleTag>>();
  BOOST_TEST(&created2.Value() != &created1);
  {
    auto scope = provider.GetScope();
    auto &created3 = scope.GetInstance<SimpleStruct>();
    BOOST_TEST(&created3 != &created1);
    BOOST_TEST(&created3 != &created2.Value());
    auto &created4 = scope.GetInstance<SimpleStruct>();
    BOOST_TEST(&created4 == &created3);
    auto created5 = scope.GetInstance<ac::registry::Tagged<SimpleStruct, SimpleTag>>();
    BOOST_TEST(&created5.Value() != &created3);
    BOOST_TEST(&created5.Value() != &created2.Value());
    BOOST_TEST(&created5.Value() != &created1);
    auto created6 = scope.GetInstance<ac::registry::Tagged<SimpleStruct, SimpleTag>>();
    BOOST_TEST(&created6.Value() == &created5.Value());
  }
  {
    auto scope = provider.GetScope();
    auto &created7 = scope.GetInstance<SimpleStruct>();
    BOOST_TEST(&created7 != &created1);
    BOOST_TEST(&created7 != &created2.Value());
    auto created8 = scope.GetInstance<ac::registry::Tagged<SimpleStruct, SimpleTag>>();
    BOOST_TEST(&created8.Value() != &created7);
    BOOST_TEST(&created8.Value() != &created2.Value());
    BOOST_TEST(&created8.Value() != &created1);
    auto &created9 = scope.GetInstance<SimpleStruct>();
    BOOST_TEST(&created9 == &created7);
    auto created10 = scope.GetInstance<ac::registry::Tagged<SimpleStruct, SimpleTag>>();
    BOOST_TEST(&created10.Value() == &created8.Value());
  }
}

BOOST_AUTO_TEST_CASE(create_tagged_transient_instance_in_scopes)
{
  ac::registry::Registry registry;
  ac::registry::Provider provider(registry);
  registry.AddTransient<SimpleStruct>();

  auto &created1 = provider.GetInstance<SimpleStruct>();
  auto created2 = provider.GetInstance<ac::registry::Tagged<SimpleStruct, SimpleTag>>();
  BOOST_TEST(&created1 != &created2.Value());
  {
    auto scope = provider.GetScope();
    auto &created3 = scope.GetInstance<SimpleStruct>();
    BOOST_TEST(&created3 != &created1);
    BOOST_TEST(&created3 != &created2.Value());
    auto created4 = scope.GetInstance<ac::registry::Tagged<SimpleStruct, SimpleTag>>();
    BOOST_TEST(&created4.Value() != &created3);
    BOOST_TEST(&created4.Value() != &created2.Value());
    BOOST_TEST(&created4.Value() != &created1);
  }
  {
    auto scope = provider.GetScope();
    auto &created5 = scope.GetInstance<SimpleStruct>();
    BOOST_TEST(&created5 != &created1);
    BOOST_TEST(&created5 != &created2.Value());
    auto created6 = scope.GetInstance<ac::registry::Tagged<SimpleStruct, SimpleTag>>();
    BOOST_TEST(&created6.Value() != &created5);
    BOOST_TEST(&created6.Value() != &created2.Value());
    BOOST_TEST(&created6.Value() != &created1);
  }
}

#pragma clang diagnostic pop
