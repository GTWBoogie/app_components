#include "signal_handler.h"

#include <cstdlib>
#include <csignal>
#include <mutex>
#include <vector>

namespace {

std::mutex _sources_mutex;
std::vector<ac::util::stop_source *> _sources;

} // namespace

namespace ac::components {

void HandleSignal(int signal)
{
  std::lock_guard<std::mutex> lock(_sources_mutex);
  bool already_requested = false;
  for(auto* source : _sources)
    already_requested |= !source->request_stop();
  if (already_requested)
    std::_Exit(EXIT_FAILURE);
}

SignalHandler::SignalHandler(util::stop_source& source)
{
  std::lock_guard<std::mutex> lock(_sources_mutex);
  _sources.push_back(&source);
  std::signal(SIGTERM, HandleSignal);
  std::signal(SIGINT, HandleSignal);
}

} // ac::components
