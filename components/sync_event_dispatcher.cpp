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

size_t SyncEventDispatcher::Register(std::type_index type, std::function<void(const std::any &)> function)
{
  std::unique_lock<std::shared_mutex> lock(_state->mutex);
  _state->handlers[type].push_back(function);
  return 0;
}

void SyncEventDispatcher::Emit(std::type_index type, const std::any &event)
{
  std::shared_lock<std::shared_mutex> lock(_state->mutex);
  for (auto &handler : _state->handlers[type])
    handler(event);
}

} // namespace components
