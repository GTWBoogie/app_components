#include "sync_event_dispatcher.h"

#include <map>
#include <mutex>
#include <shared_mutex>
#include <vector>

namespace components {

struct SyncEventDispatcher::SyncEventDispatcherState {
  std::map<std::type_index, std::vector<std::function<void(const std::any &)>>> handlers;
  std::shared_mutex mutex;
};

SyncEventDispatcher::SyncEventDispatcher()
 : IEventDispatcher()
 , _state(std::make_unique<SyncEventDispatcherState>())
{}

SyncEventDispatcher::~SyncEventDispatcher()
{}

IEventDispatcher::HandlerHandle SyncEventDispatcher::Register(std::type_index type, std::function<void(const std::any &)> function)
{
  std::unique_lock<std::shared_mutex> lock(_state->mutex);
  _state->handlers[type].push_back(function);
  return { .type = type, .id = _state->handlers[type].size() };
}

bool SyncEventDispatcher::Unregister(HandlerHandle handler_handle)
{
  std::unique_lock<std::shared_mutex> lock(_state->mutex);
  auto type_it = _state->handlers.find(handler_handle.type);
  if (type_it == _state->handlers.end())
    return false;
  if (type_it->second.size() < handler_handle.id)
    return false;
  _state->handlers[handler_handle.type][handler_handle.id - 1] = [](const std::any &){};
  return true;
}

void SyncEventDispatcher::Emit(std::type_index type, const std::any &event)
{
  std::shared_lock<std::shared_mutex> lock(_state->mutex);
  auto it = _state->handlers.find(type);
  if (it != _state->handlers.end()) {
    for (auto &handler: it->second)
      handler(event);
  }
}

} // namespace components
