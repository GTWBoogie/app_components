#pragma once

namespace ac::registry {

enum class Lifetime {
  Transient,
  Scoped,
  Singleton
};

} // namespace ac::registry
