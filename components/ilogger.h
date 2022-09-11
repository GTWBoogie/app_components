#pragma once

#include <string>

namespace components {

class ILogger {
public:
  virtual ~ILogger() {}

  virtual void Trace(const std::string& msg) = 0;
  virtual void Debug(const std::string& msg) = 0;
  virtual void Information(const std::string& msg) = 0;
  virtual void Warning(const std::string& msg) = 0;
  virtual void Error(const std::string& msg) = 0;
  virtual void Fatal(const std::string& msg) = 0;
};

} // components
