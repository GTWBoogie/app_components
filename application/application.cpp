#include "application/application.h"

#include "components/cout_logger.h"
#include "components/iservice.h"
#include "components/signal_handler.h"
#include "util/stop_token.h"

#include <thread>

namespace ac::application {

Application::Application(int argc, char *argv[])
 : _registry()
 , _provider(_registry)
{
}

registry::Registry &Application::Components() {
  return _registry;
}

void Application::Run() {
  util::stop_source stop_source;

  _registry.AddInstance<util::stop_source>(&stop_source, false);
  _registry.AddInstance<components::SignalHandler>(new components::SignalHandler(stop_source));
  _registry.AddTransient<util::stop_token>([&stop_source]() { return new util::stop_token(stop_source.get_token()); });
  _registry.AddInstance<registry::Registry>(&Components(), false);
  _registry.AddInstance<registry::Provider>(&_provider, false);
  _registry.TryAddSingleton<components::COutLogger, components::ILogger>();

  auto services = _provider.GetInstances<components::IService>();
  auto& logger = _provider.GetInstance<components::ILogger>();

  std::vector<std::thread> threads;
  for (components::IService &service: services) {
    threads.emplace_back([&service, &stop_source, &logger]() {
      try {
        service.Start(stop_source.get_token());
      }
      catch (std::exception &ex) {
        using namespace std::literals;
        logger.Error("Service error: "s + ex.what());
        stop_source.request_stop();
      }
      catch (...) {
        logger.Error("Service error: unknown");
        stop_source.request_stop();
      }
    });
  }

  while (!stop_source.stop_requested()) { std::this_thread::yield(); }
  for (components::IService &service: services) { service.Stop(stop_source.get_token()); }
  for (std::thread &thread: threads) { thread.join(); }
}

} // namespace ac::application
