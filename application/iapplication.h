#pragma once

class Registry;

class IApplication
{
public:
  virtual Registry& Components() = 0;
  virtual void Run() = 0;
};
