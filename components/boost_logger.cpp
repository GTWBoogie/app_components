#include "boost_logger.h"

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>

namespace components {

BoostLogger::BoostLogger()
 : ILogger()
{
  SetLevel(_level);
}

void BoostLogger::Trace(const std::string &msg) {
  BOOST_LOG_TRIVIAL(trace) << msg;
}

void BoostLogger::Debug(const std::string &msg) {
  BOOST_LOG_TRIVIAL(debug) << msg;
}

void BoostLogger::Information(const std::string &msg) {
  BOOST_LOG_TRIVIAL(info) << msg;
}

void BoostLogger::Warning(const std::string &msg) {
  BOOST_LOG_TRIVIAL(warning) << msg;
}

void BoostLogger::Error(const std::string &msg) {
  BOOST_LOG_TRIVIAL(error) << msg;
}

void BoostLogger::Fatal(const std::string &msg) {
  BOOST_LOG_TRIVIAL(fatal) << msg;
}

namespace {

boost::log::trivial::severity_level GetBoostLevel(ILogger::Level level)
{
  switch(level) {
    case ILogger::Level::Trace: return boost::log::trivial::trace;
    case ILogger::Level::Debug: return boost::log::trivial::debug;
    case ILogger::Level::Information: return boost::log::trivial::info;
    case ILogger::Level::Warning: return boost::log::trivial::warning;
    case ILogger::Level::Error: return boost::log::trivial::error;
    case ILogger::Level::Fatal: return boost::log::trivial::fatal;
  }
}

} // namespace

bool BoostLogger::ShouldLog(Level level)
{
  return _level <= level;
}

void BoostLogger::SetLevel(Level level)
{
  boost::log::trivial::severity_level boost_level = GetBoostLevel(level);
  boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost_level);
  _level = level;
}


} // components