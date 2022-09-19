#pragma once

#include "util/stop_token.h"

namespace components {

class SignalHandler {
public:
  SignalHandler(util::stop_source& source);
};

} // components
