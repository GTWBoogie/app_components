#pragma once

#include "dependency/registry.h"
#include "dependency/provider.h"
#include "application/iapplication.h"

class Application : IApplication
{
  Registry _registry;
  Provider _provider;

public:
  Application(int argc, char *argv[]);

  Registry& Components() override;

  void Run() override;
};
