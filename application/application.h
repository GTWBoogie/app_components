#pragma once

#include "application/iapplication.h"
#include "registry/registry.h"
#include "registry/provider.h"

namespace application {

class Application : IApplication {
  Registry _registry;
  Provider _provider;

public:
  Application(int argc, char *argv[]);

  Registry &Components() override;

  void Run() override;
};

} // namespace application
