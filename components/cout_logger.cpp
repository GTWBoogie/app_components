#include "cout_logger.h"

#include <iostream>
#include <mutex>

namespace {

std::mutex cout_lock;

} // namespace

namespace components {

void COutLogger::Trace(const std::string &msg) {
  if (!ShouldLog(Level::Trace)) return;
  std::lock_guard<std::mutex> lock(cout_lock);
  std::cout << "[Trace] " << msg << std::endl;
}

void COutLogger::Debug(const std::string &msg) {
  if (!ShouldLog(Level::Debug)) return;
  std::lock_guard<std::mutex> lock(cout_lock);
  std::cout << "[Debug] " << msg << std::endl;
}

void COutLogger::Information(const std::string &msg) {
  if (!ShouldLog(Level::Information)) return;
  std::lock_guard<std::mutex> lock(cout_lock);
  std::cout << "[Information] " << msg << std::endl;
}

void COutLogger::Warning(const std::string &msg) {
  if (!ShouldLog(Level::Warning)) return;
  std::lock_guard<std::mutex> lock(cout_lock);
  std::cout << "[Warning] " << msg << std::endl;
}

void COutLogger::Error(const std::string &msg) {
  if (!ShouldLog(Level::Error)) return;
  std::lock_guard<std::mutex> lock(cout_lock);
  std::cout << "[Error] " << msg << std::endl;
}

void COutLogger::Fatal(const std::string &msg) {
  if (!ShouldLog(Level::Fatal)) return;
  std::lock_guard<std::mutex> lock(cout_lock);
  std::cout << "[Fatal] " << msg << std::endl;
}

} // components