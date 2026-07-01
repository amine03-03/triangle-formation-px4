// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from formation_interfaces:msg/DroneState.idl
// generated code does not contain a copyright notice

#ifndef FORMATION_INTERFACES__MSG__DETAIL__DRONE_STATE__BUILDER_HPP_
#define FORMATION_INTERFACES__MSG__DETAIL__DRONE_STATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "formation_interfaces/msg/detail/drone_state__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace formation_interfaces
{

namespace msg
{

namespace builder
{

class Init_DroneState_valid
{
public:
  explicit Init_DroneState_valid(::formation_interfaces::msg::DroneState & msg)
  : msg_(msg)
  {}
  ::formation_interfaces::msg::DroneState valid(::formation_interfaces::msg::DroneState::_valid_type arg)
  {
    msg_.valid = std::move(arg);
    return std::move(msg_);
  }

private:
  ::formation_interfaces::msg::DroneState msg_;
};

class Init_DroneState_yaw
{
public:
  explicit Init_DroneState_yaw(::formation_interfaces::msg::DroneState & msg)
  : msg_(msg)
  {}
  Init_DroneState_valid yaw(::formation_interfaces::msg::DroneState::_yaw_type arg)
  {
    msg_.yaw = std::move(arg);
    return Init_DroneState_valid(msg_);
  }

private:
  ::formation_interfaces::msg::DroneState msg_;
};

class Init_DroneState_position
{
public:
  explicit Init_DroneState_position(::formation_interfaces::msg::DroneState & msg)
  : msg_(msg)
  {}
  Init_DroneState_yaw position(::formation_interfaces::msg::DroneState::_position_type arg)
  {
    msg_.position = std::move(arg);
    return Init_DroneState_yaw(msg_);
  }

private:
  ::formation_interfaces::msg::DroneState msg_;
};

class Init_DroneState_index
{
public:
  explicit Init_DroneState_index(::formation_interfaces::msg::DroneState & msg)
  : msg_(msg)
  {}
  Init_DroneState_position index(::formation_interfaces::msg::DroneState::_index_type arg)
  {
    msg_.index = std::move(arg);
    return Init_DroneState_position(msg_);
  }

private:
  ::formation_interfaces::msg::DroneState msg_;
};

class Init_DroneState_timestamp
{
public:
  Init_DroneState_timestamp()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_DroneState_index timestamp(::formation_interfaces::msg::DroneState::_timestamp_type arg)
  {
    msg_.timestamp = std::move(arg);
    return Init_DroneState_index(msg_);
  }

private:
  ::formation_interfaces::msg::DroneState msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::formation_interfaces::msg::DroneState>()
{
  return formation_interfaces::msg::builder::Init_DroneState_timestamp();
}

}  // namespace formation_interfaces

#endif  // FORMATION_INTERFACES__MSG__DETAIL__DRONE_STATE__BUILDER_HPP_
