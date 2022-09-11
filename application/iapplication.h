#pragma once

class Registry;

namespace components {

class IApplication
{
public:
  virtual Registry& Components() = 0;
  virtual void Run() = 0;
};

} // namespace components
