#include "application/application.h"

#include "components/iservice.h"
#include "util/debug_log.h"
#include "util/stop_token.h"

#include <thread>

namespace components {

Application::Application(int argc, char *argv[])
        : _registry(), _provider(_registry) {
}

Registry &Application::Components() {
  return _registry;
}

void Application::Run() {
  util::stop_source stop_source;

  _registry.AddInstance(&stop_source, false);
  _registry.AddTransient([&stop_source]() { return new util::stop_token(stop_source.get_token()); });
  _registry.AddInstance(&Components(), false);
  _registry.AddInstance(&_provider, false);

  auto services = _provider.GetInstances<components::IService>();

  std::vector<std::thread> threads;
  for (components::IService &service: services) {
    threads.emplace_back([&service, &stop_source]() {
      try {
        service.Start(stop_source.get_token());
      }
      catch (std::exception &ex) {
        // todo: change to logger
        DLOG("Service error: " << ex.what());
        stop_source.request_stop();
      }
      catch (...) {
        // todo: change to logger
        DLOG("Service error: unknown");
        stop_source.request_stop();
      }
    });
  }

  while (!stop_source.stop_requested()) { std::this_thread::yield(); }
  for (IService &service: services) { service.Stop(stop_source.get_token()); }
  for (std::thread &thread: threads) { thread.join(); }
}

} // namespace components
