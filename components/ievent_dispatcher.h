#pragma once

#include <any>
#include <functional>
#include <typeindex>

namespace components {

class IEventDispatcher {
public:
  IEventDispatcher() = default;
  virtual ~IEventDispatcher() = default;

  template<typename Event>
  size_t RegisterHandler(std::function<void (const Event&)> function)
  {
    return Register(typeid(Event), [function](const std::any& event) { function(std::any_cast<Event>(event)); });
  }

  template<typename Event>
  void EmitEvent(const Event& event)
  {
    Emit(typeid(Event), event);
  }

protected:
  virtual size_t Register(std::type_index type, std::function<void (const std::any&)> function) = 0;
  virtual void Emit(std::type_index type, const std::any& event) = 0;
};

} // namespace components
