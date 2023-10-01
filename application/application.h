#pragma once

#include "application/iapplication.h"
#include "registry/registry.h"
#include "registry/provider.h"

namespace ac::application {

class Application : IApplication {
  registry::Registry _registry;
  registry::Provider _provider;

public:
  Application(int argc, char *argv[]);

  registry::Registry &Components() override;

  void Run() override;
};

} // namespace ac::application
