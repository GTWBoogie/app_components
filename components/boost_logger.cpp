#include "boost_logger.h"

#include <boost/log/trivial.hpp>

namespace components {
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

} // components