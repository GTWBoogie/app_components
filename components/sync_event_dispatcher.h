#pragma once

#include "ievent_dispatcher.h"

#include <memory>

namespace components {

class SyncEventDispatcher : public IEventDispatcher {
  struct SyncEventDispatcherState;
public:
  SyncEventDispatcher();
  ~SyncEventDispatcher();

protected:
  size_t Register(std::type_index type, std::function<void(const std::any &)> function) override;
  void Emit(std::type_index type, const std::any &event) override;

private:
  std::unique_ptr<SyncEventDispatcherState> _state;
};

} // namespace components
