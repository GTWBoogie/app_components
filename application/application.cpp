#include "application/application.h"

#include "util/stop_token.h"
#include "application/iservice.h"

#include <thread>

Application::Application(int argc, char *argv[])
 : _registry()
 , _provider(_registry)
{
}

Registry& Application::Components()
{
  return _registry;
}

void Application::Run()
{
  util::stop_source stop_source;

  _registry.AddInstance(&stop_source, false);
  _registry.AddTransient([&stop_source]() { return new util::stop_token(stop_source.get_token()); });
  _registry.AddInstance(&Components(), false);
  _registry.AddInstance(&_provider, false);

  auto services = _provider.GetInstances<IService>();

  std::vector<std::thread> threads;
  for (IService& service : services)
  {
    threads.emplace_back([&service, &stop_source](){
      try {
        service.Start(stop_source.get_token());
      }
      catch(std::exception &ex)
      {
        std::cout << "Service error: " << ex.what() << std::endl;
        stop_source.request_stop();
      }
      catch(...)
      {
        std::cout << "Service error: unknown" << std::endl;
        stop_source.request_stop();
      }
    });
  }

  while (!stop_source.stop_requested()) { std::this_thread::yield(); }
  for (IService& service : services)    { service.Stop(stop_source.get_token()); }
  for (std::thread& thread : threads)   { thread.join(); }
}

