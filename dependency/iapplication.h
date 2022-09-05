#pragma once

class ComponentRegistry;

class IApplication
{
public:
  virtual ComponentRegistry& Components() = 0;
  virtual void Run() = 0;
};
