#pragma once

#include "ilogger.h"

namespace ac::components {

class COutLogger : public ILogger {
public:
  COutLogger() = default;

  void Trace(const std::string &msg) override;
  void Debug(const std::string &msg) override;
  void Information(const std::string &msg) override;
  void Warning(const std::string &msg) override;
  void Error(const std::string &msg) override;
  void Fatal(const std::string &msg) override;
};

} // ac:components
