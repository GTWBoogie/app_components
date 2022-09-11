#include "cout_logger.h"

#include <iostream>
#include <mutex>

namespace {

std::mutex cout_lock;

} // namespace

namespace components {

void COutLogger::Trace(const std::string &msg) {
  std::lock_guard<std::mutex> lock(cout_lock);
  std::cout << msg << std::endl;
}

void COutLogger::Debug(const std::string &msg) {
  std::lock_guard<std::mutex> lock(cout_lock);
  std::cout << msg << std::endl;
}

void COutLogger::Information(const std::string &msg) {
  std::lock_guard<std::mutex> lock(cout_lock);
  std::cout << msg << std::endl;
}

void COutLogger::Warning(const std::string &msg) {
  std::lock_guard<std::mutex> lock(cout_lock);
  std::cout << msg << std::endl;
}

void COutLogger::Error(const std::string &msg) {
  std::lock_guard<std::mutex> lock(cout_lock);
  std::cout << msg << std::endl;
}

void COutLogger::Fatal(const std::string &msg) {
  std::lock_guard<std::mutex> lock(cout_lock);
  std::cout << msg << std::endl;
}

} // components