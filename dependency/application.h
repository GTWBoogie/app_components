#pragma once

#include "component_registry.h"
#include "component_provider.h"
#include "iapplication.h"

class Application : IApplication
{
  ComponentRegistry _registry;
  ComponentProvider _provider;

public:
  Application(int argc, char *argv[]);

  ComponentRegistry& Components() override;

  void Run() override;
};
