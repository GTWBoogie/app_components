#pragma once

#include "ievent_dispatcher.h"
#include "util/stop_token.h"

#include <memory>

namespace components {

class ASyncEventDispatcher : public IEventDispatcher {
  struct ASyncEventDispatcherState;
public:
  ASyncEventDispatcher(util::stop_token stop_token);
  ~ASyncEventDispatcher();

protected:
  IEventDispatcher::HandlerHandle Register(std::type_index type, std::function<void(const std::any &)> function) override;
  bool Unregister(HandlerHandle handler_handle) override;
  void Emit(std::type_index type, const std::any &event) override;

private:
  std::unique_ptr<ASyncEventDispatcherState> _state;
};

} // components
