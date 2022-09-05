#pragma once

#include <memory>

namespace util {

class stop_source;
class stop_state;

class stop_token {
  std::shared_ptr<stop_state> _state;
  stop_token(std::shared_ptr<stop_state> state);

  friend class stop_source;
public:
  stop_token();
  stop_token(const stop_token& other);

  [[nodiscard]] bool stop_requested() const noexcept;
  [[nodiscard]] bool stop_possible() const noexcept;
};

class stop_source {
  std::shared_ptr<stop_state> _state;
public:
  stop_source();

  [[nodiscard]] stop_token get_token() const noexcept;
  [[nodiscard]] bool stop_requested() const noexcept;
  [[nodiscard]] bool stop_possible() const noexcept;
  bool request_stop() noexcept;
};

} // namespace util

