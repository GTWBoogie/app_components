#include "application/application.h"
#include "application/iservice.h"

#include <iostream>
#include <thread>

class ICounterProvider {
public:
  virtual ~ICounterProvider() = default;

  virtual size_t Get() = 0;
};

class CounterProvider : public ICounterProvider{
  size_t _counter = 0;
public:
  CounterProvider() = default;

  size_t Get() override { return ++_counter; }
};

class Counter {
  size_t _counter;
public:
  explicit Counter(ICounterProvider& provider) : _counter(provider.Get()) { }

  [[nodiscard]] size_t Get() const { return _counter; }
};

class Printer {
  const Counter& _counter;
public:
  explicit Printer(const Counter& counter)
   : _counter(counter)
  {
  }

  void DoStuff()
  {
    std::cout << "Hello! " << _counter.Get() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
  }
};

class PrintingService : public application::IService
{
  Provider& _provider;
public:
  explicit PrintingService(Provider& provider)
   : _provider(provider)
  {
  }

  void Start(util::stop_token stop_token) override
  {
    std::cout << "Starting PrintingService " << std::endl;
    while (!stop_token.stop_requested())
    {
      auto scope = _provider.GetScope();
      auto& printer = scope.GetInstance<Printer>();
      printer.DoStuff();
    }
  }

  void Stop(util::stop_token stop_token) override
  {
    std::cout << "Stopping PrintingService " << std::endl;
  }
};

class KillingService : public application::IService
{
  util::stop_source& _source;
public:
  explicit KillingService(util::stop_source& source)
   : _source(source)
  {
  }

  static KillingService* Create(util::stop_source& source)
  {
    return new KillingService(source);
  }

  void Start(util::stop_token stop_token) override
  {
    std::cout << "Starting KillingService " << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(2));
    _source.request_stop();
  }

  void Stop(util::stop_token stop_token) override
  {
    std::cout << "Stopping KillingService " << std::endl;
  }
};

struct Parameters {
    int a;
    bool b;
    std::string c;
};

void PrintParams(const Parameters& params)
{
  std::cout << params.a << " " << params.b << " " << params.c << std::endl;
}

int main (int argc, char *argv[])
{
  try {
    application::Application app(argc, argv);

    app.Components().AddSingleton<CounterProvider, ICounterProvider>();

    app.Components().AddSingleton<application::IService>([](Provider& provider) { return new PrintingService(provider); });

    app.Components().AddSingleton<KillingService, application::IService>();

    app.Components().AddScoped([](Counter& counter) { return new Printer(counter); });
    app.Components().AddTransient([](ICounterProvider& provider) { return new Counter(provider); });
    app.Run();

    std::cout << "Success!" << std::endl;
  }
  catch(const std::exception& ex)
  {
    std::cout << "Err: " << ex.what() << std::endl;
  }
  catch(...)
  {
    std::cout << "Err?" << std::endl;
  }

  PrintParams({ .a = 3, .b = false, .c = "ds"});

  return 0;
}
