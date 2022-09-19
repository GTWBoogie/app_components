#include "signal_handler.h"

#include <csignal>
#include <vector>

namespace {

std::vector<util::stop_source *> _sources;

} // namespace

namespace components {

void HandleSignal(int signal)
{
  bool already_requested = false;
  for(auto* source : _sources)
    already_requested |= !source->request_stop();
  if (already_requested)
    std::_Exit(EXIT_FAILURE);
}

SignalHandler::SignalHandler(util::stop_source& source)
{
  _sources.push_back(&source);
  std::signal(SIGTERM, HandleSignal);
  std::signal(SIGINT, HandleSignal);
}

} // components
