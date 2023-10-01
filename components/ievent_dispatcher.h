#pragma once

#include <any>
#include <functional>
#include <typeindex>

namespace ac::components {

class IEventDispatcher {
public:
  struct HandlerHandle {
    std::type_index type;
    size_t id;
  };

  IEventDispatcher() = default;
  virtual ~IEventDispatcher() = default;

  template<typename Event>
  HandlerHandle RegisterHandler(std::function<void (const Event&)> function)
  {
    return Register(typeid(Event), [function](const std::any& event) { function(std::any_cast<Event>(event)); });
  }

  bool UnregisterHandler(HandlerHandle handler_handle)
  {
    return Unregister(handler_handle);
  }

  template<typename Event>
  void EmitEvent(const Event& event)
  {
    Emit(typeid(Event), event);
  }

protected:
  virtual HandlerHandle Register(std::type_index type, std::function<void (const std::any&)> function) = 0;
  virtual bool Unregister(HandlerHandle handler_handle) = 0;
  virtual void Emit(std::type_index type, const std::any& event) = 0;
};

} // namespace ac::components
