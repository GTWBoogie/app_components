#pragma once

#include "util/stop_token.h"

class IService
{
public:
  virtual ~IService() {};

  virtual void Start(util::stop_token stop_token) = 0;
  virtual void Stop(util::stop_token stop_token) = 0;
};
