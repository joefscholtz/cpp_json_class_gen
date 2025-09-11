#pragma once
#include <string>
namespace events {

class BaseEvent {
public:
  BaseEvent() {}
  virtual ~BaseEvent() = default;

  std::string _base_class_atr = "value";
};

} // namespace events
