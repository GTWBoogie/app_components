#include "application/application.h"
#include "components/boost_logger.h"
#include "components/cout_logger.h"
#include "components/iservice.h"

#include <iostream>
#include <thread>

class ICounterProvider {
public:
  virtual ~ICounterProvider() = default;

  virtual size_t Get() = 0;
};

class CounterProvider : public ICounterProvider {
  size_t _counter = 0;

public:
  CounterProvider() = default;

  size_t Get() override { return ++_counter; }
};

class Counter {
  size_t _counter;

public:
  explicit Counter(ICounterProvider &provider) : _counter(provider.Get()) {}

  [[nodiscard]] size_t Get() const { return _counter; }
};

class Printer {
  const Counter &_counter;
  ac::components::ILogger &_logger;

public:
  explicit Printer(const Counter &counter, ac::components::ILogger &logger)
      : _counter(counter), _logger(logger) {
    _logger.Trace(std::string("Printer Logger address ") +
                  std::to_string(size_t(&_logger)));
  }

  void DoStuff() {
    _logger.Information("Hello! " + std::to_string(_counter.Get()));
  }
};

class PrintingService : public ac::components::IService {
  ac::registry::Provider &_provider;
  ac::components::ILogger &_logger;

public:
  explicit PrintingService(ac::registry::Provider &provider,
                           ac::registry::Tagged<ac::components::ILogger, PrintingService> &logger)
      : _provider(provider), _logger(logger.Value()) {
    _logger.Trace(std::string("PrintingService Logger address ") +
                  std::to_string(size_t(&_logger)));
  }

  static PrintingService *
  Create(ac::registry::Provider &provider,
         ac::registry::Tagged<ac::components::ILogger, PrintingService> &logger) {
    return new PrintingService(provider, logger);
  }

  void Start(ac::util::stop_token stop_token) override {
    _logger.Debug("Starting PrintingService");
    while (!stop_token.stop_requested()) {
      auto scope = _provider.GetScope();
      auto &printer = scope.GetInstance<Printer>();
      printer.DoStuff();
      std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
  }

  void Stop(ac::util::stop_token stop_token) override {
    _logger.Debug("Stopping PrintingService");
  }
};

class KillingService : public ac::components::IService {
  ac::util::stop_source &_source;
  ac::components::ILogger &_logger;

public:
  CONSTRUCTOR(KillingService(ac::util::stop_source &source,
                             ac::components::ILogger &logger))
      : _source(source), _logger(logger) {
    _logger.Trace(std::string("KillingService Logger address ") +
                  std::to_string(size_t(&_logger)));
  }

  void Start(ac::util::stop_token stop_token) override {
    _logger.Debug("Starting KillingService");

    size_t counter = 0;

    while (counter++ < 100 && !stop_token.stop_requested())
      std::this_thread::sleep_for(std::chrono::milliseconds(100));

    _source.request_stop();
  }

  void Stop(ac::util::stop_token stop_token) override {
    _logger.Debug("Stopping KillingService");
  }
};

int main(int argc, char *argv[]) {
  try {
    ac::application::Application app(argc, argv);

    app.Components()
        .AddSingleton<ac::components::BoostLogger, ac::components::ILogger>();
    app.Components().AddSingleton<CounterProvider, ICounterProvider>();
    app.Components().AddSingleton<PrintingService, ac::components::IService>();
    app.Components().AddSingleton<KillingService, ac::components::IService>();
    app.Components().AddScoped<Printer>(
        [](Counter &counter, ac::components::ILogger &logger) {
          return new Printer(counter, logger);
        });
    app.Components().AddTransient<Counter>(
        [](ICounterProvider &provider) { return new Counter(provider); });
    app.Run();

    std::cout << "Success!" << std::endl;
  } catch (const std::exception &ex) {
    std::cout << "Err: " << ex.what() << std::endl;
  } catch (...) {
    std::cout << "Err?" << std::endl;
  }

  return 0;
}
