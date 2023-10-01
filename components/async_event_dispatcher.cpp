#include "async_event_dispatcher.h"

#include "util/lockless_lifo_queue.h"

#include <chrono>
#include <map>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>

namespace {

using EventType = std::pair<std::type_index, const std::any>;

}

namespace ac::components {

struct ASyncEventDispatcher::ASyncEventDispatcherState {
  std::map<std::type_index, std::vector<std::function<void(const std::any &)>>> handlers;
  std::shared_mutex handlers_mutex;

  util::LocklessLIFOQueue<EventType> event_queue;

  std::thread thread;
};

ASyncEventDispatcher::ASyncEventDispatcher(util::stop_token stop_token)
 : IEventDispatcher()
 , _state(std::make_unique<ASyncEventDispatcherState>())
{
  _state->thread = std::thread([this, stop_token]() {
    auto process_events = [this](const EventType& event_type) {
      auto it = _state->handlers.find(event_type.first);
      if (it != _state->handlers.end())
        for (auto &handler: it->second)
          handler(event_type.second);
    };

    while(!stop_token.stop_requested())
    {
      std::this_thread::yield();
      while (!_state->event_queue.empty()) {
        std::shared_lock<std::shared_mutex> handlers_lock(_state->handlers_mutex);
        _state->event_queue.consume_all(process_events);
      }
    }
    std::shared_lock<std::shared_mutex> handlers_lock(_state->handlers_mutex);
    _state->event_queue.consume_all(process_events);
  });
}

ASyncEventDispatcher::~ASyncEventDispatcher()
{
  _state->thread.join();
}

IEventDispatcher::HandlerHandle ASyncEventDispatcher::Register(std::type_index type, std::function<void(const std::any &)> function)
{
  std::unique_lock<std::shared_mutex> lock(_state->handlers_mutex);
  _state->handlers[type].push_back(function);
  return { .type = type, .id = _state->handlers[type].size() };
}

bool ASyncEventDispatcher::Unregister(HandlerHandle handler_handle)
{
  std::unique_lock<std::shared_mutex> lock(_state->handlers_mutex);
  auto type_it = _state->handlers.find(handler_handle.type);
  if (type_it == _state->handlers.end())
    return false;
  if (type_it->second.size() < handler_handle.id)
    return false;
  _state->handlers[handler_handle.type][handler_handle.id - 1] = [](const std::any &){};
  return true;
}

void ASyncEventDispatcher::Emit(std::type_index type, const std::any &event)
{
  _state->event_queue.push({type, event});
}

} // ac::namespace components
