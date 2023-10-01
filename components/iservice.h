#pragma once

#include "util/stop_token.h"

namespace ac::components {

class IService
{
public:
  virtual ~IService() = default;

  virtual void Start(util::stop_token stop_token) = 0;
  virtual void Stop(util::stop_token stop_token) = 0;
};

} // namespace ac::components
