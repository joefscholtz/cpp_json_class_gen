#pragma once
#include <string>
namespace events {

class BaseEvent {
public:
  BaseEvent(std::string example_attribute = std::string("base class value"))
      : _example_attribute(example_attribute) {}
  virtual ~BaseEvent() = default;

  std::string _example_attribute;
};

} // namespace events
