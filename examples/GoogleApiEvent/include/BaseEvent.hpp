#pragma once
#include <memory>
#include <string>

namespace events {

namespace events {

class BaseEvent {
public:
  BaseEvent(std::string example_attribute = "base class value")
      : _example_attribute(std::move(example_attribute)) {}

  virtual ~BaseEvent() = default;

  // Force all derived classes to implement clone method it.
  virtual std::unique_ptr<BaseEvent> clone() const = 0;

  const std::string &get_example_attribute() const {
    return this->_example_attribute;
  }

private:
  std::string _example_attribute;
};

} // namespace events
