#include "stop_token.h"

#include <atomic>
#include <utility>

namespace util {

class stop_state {
public:
  std::atomic<bool> _stop_requested = false;
};

stop_token::stop_token()
 : _state(std::make_shared<stop_state>())
{
}

stop_token::stop_token(std::shared_ptr<stop_state> state)
 : _state(std::move(state))
{
}

[[nodiscard]] bool stop_token::stop_requested() const noexcept
{
  return _state->_stop_requested;
}

[[nodiscard]] bool stop_token::stop_possible() const noexcept
{
  return !stop_requested();
}

stop_source::stop_source()
 : _state(std::make_shared<stop_state>())
{
}

[[nodiscard]] stop_token stop_source::get_token() const noexcept
{
  return stop_token(_state);
}

[[nodiscard]] bool stop_source::stop_requested() const noexcept
{
  return _state->_stop_requested;
}

[[nodiscard]] bool stop_source::stop_possible() const noexcept
{
  return !stop_requested();
}

bool stop_source::request_stop() noexcept
{
  bool previous = _state->_stop_requested.exchange(true);
  return !previous;
}

} // namespace util

