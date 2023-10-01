#include "provider.h"
#include "registry.h"

#define BOOST_TEST_MODULE dependency injection tests
#include <boost/test/included/unit_test.hpp>
#include <utility>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"

struct StructWithoutDependencies {};

BOOST_AUTO_TEST_CASE(no_dependencies_struct_creation)
{
  ac::registry::Registry registry;
  ac::registry::Provider provider(registry);

  registry.AddSingleton<StructWithoutDependencies>();

  auto &created1 = provider.GetInstance<StructWithoutDependencies>();
  auto &created2 = provider.GetInstance<StructWithoutDependencies>();

  BOOST_TEST(&created1 == &created2);
}

struct StructWithOneDependency {
  explicit StructWithOneDependency(StructWithoutDependencies& dep)
          : _dependency(dep)
  {
  }

  StructWithoutDependencies& _dependency;
};

BOOST_AUTO_TEST_CASE(struct_with_one_dependency_creation_singletons)
{
  ac::registry::Registry registry;
  ac::registry::Provider provider(registry);

  registry.AddSingleton<StructWithOneDependency>([](StructWithoutDependencies& dep) { return new StructWithOneDependency(dep); });
  registry.AddSingleton<StructWithoutDependencies>();

  auto &created1 = provider.GetInstance<StructWithOneDependency>();
  auto &created2 = provider.GetInstance<StructWithOneDependency>();

  BOOST_TEST(&created1 == &created2);
  BOOST_TEST(&created1._dependency == &created2._dependency);
}

BOOST_AUTO_TEST_CASE(struct_with_one_dependency_creation_transients)
{
  ac::registry::Registry registry;
  ac::registry::Provider provider(registry);

  registry.AddTransient<StructWithOneDependency>([](StructWithoutDependencies& dep) { return new StructWithOneDependency(dep); });
  registry.AddSingleton<StructWithoutDependencies>();

  auto &created1 = provider.GetInstance<StructWithOneDependency>();
  auto &created2 = provider.GetInstance<StructWithOneDependency>();

  BOOST_TEST(&created1 != &created2);
  BOOST_TEST(&created1._dependency == &created2._dependency);
}

struct StructWithOneDependencyTagged1 {
  explicit StructWithOneDependencyTagged1(ac::registry::Tagged<StructWithoutDependencies, StructWithOneDependencyTagged1>& dep)
          : _dependency(dep.Value())
  {
  }

  static StructWithOneDependencyTagged1* Create(ac::registry::Tagged<StructWithoutDependencies, StructWithOneDependencyTagged1>& dep)
  {
    return new StructWithOneDependencyTagged1(dep);
  }

  StructWithoutDependencies& _dependency;
};

struct StructWithOneDependencyTagged2 {
  explicit StructWithOneDependencyTagged2(ac::registry::Tagged<StructWithoutDependencies, StructWithOneDependencyTagged2>& dep)
          : _dependency(dep.Value())
  {
  }

  static StructWithOneDependencyTagged2* Create(ac::registry::Tagged<StructWithoutDependencies, StructWithOneDependencyTagged2>& dep)
  {
    return new StructWithOneDependencyTagged2(dep);
  }

  StructWithoutDependencies& _dependency;
};

BOOST_AUTO_TEST_CASE(struct_with_one_dependency_creation_tagged_singletons)
{
  ac::registry::Registry registry;
  ac::registry::Provider provider(registry);

  registry.AddSingleton<StructWithOneDependencyTagged1>();
  registry.AddSingleton<StructWithOneDependencyTagged2>();
  registry.AddSingleton<StructWithoutDependencies>();

  auto &created1 = provider.GetInstance<StructWithOneDependencyTagged1>();
  auto &created2 = provider.GetInstance<StructWithOneDependencyTagged2>();

  BOOST_TEST(&created1._dependency != &created2._dependency);
}

struct StructWithOneDependencyVector {
  explicit StructWithOneDependencyVector(ac::registry::ComponentContainer<StructWithoutDependencies> dep)
   : _dependency(std::move(dep))
  {
  }

  static StructWithOneDependencyVector* Create(ac::registry::ComponentContainer<StructWithoutDependencies> dep)
  {
    return new StructWithOneDependencyVector(std::move(dep));
  }

  ac::registry::ComponentContainer<StructWithoutDependencies> _dependency;
};

BOOST_AUTO_TEST_CASE(struct_with_one_dependency_vector_creation_singletons)
{
  ac::registry::Registry registry;
  ac::registry::Provider provider(registry);

  registry.AddSingleton<StructWithOneDependencyVector>();
  registry.AddSingleton<StructWithoutDependencies>();

  auto &created = provider.GetInstance<StructWithOneDependencyVector>();

  BOOST_TEST(created._dependency.size() == 1);
}

BOOST_AUTO_TEST_CASE(struct_with_one_dependency_vector_creation_transients)
{
  ac::registry::Registry registry;
  ac::registry::Provider provider(registry);

  registry.AddTransient<StructWithOneDependencyVector>();
  registry.AddSingleton<StructWithoutDependencies>();

  auto &created1 = provider.GetInstance<StructWithOneDependencyVector>();
  auto &created2 = provider.GetInstance<StructWithOneDependencyVector>();
  BOOST_TEST(created1._dependency.size() == 1);
  BOOST_TEST(created2._dependency.size() == 1);
  BOOST_TEST(&created1._dependency.front().get() == &created2._dependency.front().get());
}

struct StructWithOneDependencyVectorTagged {
  explicit StructWithOneDependencyVectorTagged(ac::registry::Tagged<ac::registry::ComponentContainer<StructWithoutDependencies>, StructWithOneDependencyVectorTagged> dep)
   : _dependency(std::move(dep.Value()))
  {
  }

  static StructWithOneDependencyVectorTagged* Create(ac::registry::Tagged<ac::registry::ComponentContainer<StructWithoutDependencies>, StructWithOneDependencyVectorTagged> dep)
  {
    return new StructWithOneDependencyVectorTagged(std::move(dep));
  }

  ac::registry::ComponentContainer<StructWithoutDependencies> _dependency;
};

BOOST_AUTO_TEST_CASE(struct_with_one_dependency_vector_tagged_creation_singletons)
{
  ac::registry::Registry registry;
  ac::registry::Provider provider(registry);

  registry.AddTransient<StructWithOneDependencyVector>();
  registry.AddTransient<StructWithOneDependencyVectorTagged>();
  registry.AddSingleton<StructWithoutDependencies>();
  registry.AddSingleton<StructWithoutDependencies>();

  auto &created1 = provider.GetInstance<StructWithOneDependencyVectorTagged>();
  auto &created2 = provider.GetInstance<StructWithOneDependencyVectorTagged>();
  auto &created3 = provider.GetInstance<StructWithOneDependencyVector>();
  auto &created4 = provider.GetInstance<StructWithOneDependencyVector>();

  BOOST_TEST(&created1 != &created2);
  BOOST_TEST(created1._dependency.size() == 2);
  BOOST_TEST(&created1._dependency.front().get() != &created1._dependency.back().get());
  BOOST_TEST(&created1._dependency.front().get() == &created2._dependency.front().get());
  BOOST_TEST(&created1._dependency.back().get() == &created2._dependency.back().get());
  BOOST_TEST(&created3 != &created4);
  BOOST_TEST(created3._dependency.size() == 2);
  BOOST_TEST(&created3._dependency.front().get() != &created3._dependency.back().get());
  BOOST_TEST(&created3._dependency.front().get() == &created4._dependency.front().get());
  BOOST_TEST(&created3._dependency.back().get() == &created4._dependency.back().get());

  BOOST_TEST(&created1._dependency.front().get() != &created3._dependency.front().get());
  BOOST_TEST(&created1._dependency.front().get() != &created3._dependency.back().get());
  BOOST_TEST(&created1._dependency.back().get() != &created3._dependency.back().get());
  BOOST_TEST(&created1._dependency.back().get() != &created3._dependency.front().get());
}

#pragma clang diagnostic pop
