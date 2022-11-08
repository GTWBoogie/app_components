#pragma once

#include <string>

namespace components {

class ILogger {
public:
  enum class Level {
    Trace,
    Debug,
    Information,
    Warning,
    Error,
    Fatal
  };

  ILogger() : _level(Level::Information) {}
  virtual ~ILogger() {}

  virtual void Trace(const std::string& msg) = 0;
  virtual void Debug(const std::string& msg) = 0;
  virtual void Information(const std::string& msg) = 0;
  virtual void Warning(const std::string& msg) = 0;
  virtual void Error(const std::string& msg) = 0;
  virtual void Fatal(const std::string& msg) = 0;

  virtual bool ShouldLog(Level level) { return _level <= level; }
  virtual void SetLevel(Level level) { _level = level; }

protected:
  Level _level;
};

} // components
