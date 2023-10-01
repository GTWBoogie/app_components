#pragma once

namespace ac::registry {

class Registry;

} // namespace ac::registry

namespace ac::application {

class IApplication
{
public:
  virtual registry::Registry& Components() = 0;
  virtual void Run() = 0;
};

} // namespace ac::application
