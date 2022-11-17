#include "async_event_dispatcher.h"

#include <chrono>
#include <condition_variable>
#include <map>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>

namespace {

using EventType = std::pair<std::type_index, const std::any>;

}

namespace components {

struct ASyncEventDispatcher::ASyncEventDispatcherState {
  std::map<std::type_index, std::vector<std::function<void(const std::any &)>>> handlers;
  std::shared_mutex handlers_mutex;

  // TODO: stupid implementation - replace vector with lockless queue and remove mutex
  std::vector<EventType> event_queue;
  std::mutex queue_mutex;

  std::condition_variable condition;
  std::mutex condition_mutex;

  std::thread thread;
};

ASyncEventDispatcher::ASyncEventDispatcher(util::stop_token stop_token)
 : IEventDispatcher()
 , _state(std::make_unique<ASyncEventDispatcherState>())
{
  _state->thread = std::thread([this, stop_token]() {

    auto process_events = [this](const std::vector<EventType>& queue) {
      for (auto& [type, event] : queue)
      {
        auto it = _state->handlers.find(type);
        if (it != _state->handlers.end()) {
          for (auto &handler: it->second)
            handler(event);
        }
      }
    };

    while(!stop_token.stop_requested())
    {
      std::vector<EventType> queue_copy;
      std::unique_lock<std::mutex> condition_lock(_state->condition_mutex);
      if (_state->condition.wait_for(condition_lock, std::chrono::microseconds(1)) == std::cv_status::no_timeout ||
          !_state->event_queue.empty())
      {
        if (_state->event_queue.empty())
          continue;

        {
          std::unique_lock<std::mutex> queue_lock(_state->queue_mutex);
          queue_copy.swap(_state->event_queue);
          _state->event_queue.reserve(queue_copy.capacity());
        }

        std::shared_lock<std::shared_mutex> handlers_lock(_state->handlers_mutex);
        process_events(queue_copy);
      }
    }

    std::shared_lock<std::shared_mutex> handlers_lock(_state->handlers_mutex);
    std::unique_lock<std::mutex> queue_lock(_state->queue_mutex);
    process_events(_state->event_queue);
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
  {
    std::unique_lock<std::mutex> lock(_state->queue_mutex);
    _state->event_queue.emplace_back(type, event);
  }
  _state->condition.notify_one();
}

} // namespace components
