#pragma once

#include "ilogger.h"

namespace components {

class BoostLogger : public ILogger {
public:
  BoostLogger() = default;

  void Trace(const std::string &msg) override;
  void Debug(const std::string &msg) override;
  void Information(const std::string &msg) override;
  void Warning(const std::string &msg) override;
  void Error(const std::string &msg) override;
  void Fatal(const std::string &msg) override;
};

} // components
