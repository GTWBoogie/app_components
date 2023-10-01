#pragma once

#include "util/stop_token.h"

namespace ac::components {

class SignalHandler {
public:
  SignalHandler(util::stop_source& source);
};

} // ac::components
