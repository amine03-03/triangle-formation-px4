// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from formation_interfaces:msg/LeaderState.idl
// generated code does not contain a copyright notice

#ifndef FORMATION_INTERFACES__MSG__DETAIL__LEADER_STATE__BUILDER_HPP_
#define FORMATION_INTERFACES__MSG__DETAIL__LEADER_STATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "formation_interfaces/msg/detail/leader_state__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace formation_interfaces
{

namespace msg
{

namespace builder
{

class Init_LeaderState_phase
{
public:
  explicit Init_LeaderState_phase(::formation_interfaces::msg::LeaderState & msg)
  : msg_(msg)
  {}
  ::formation_interfaces::msg::LeaderState phase(::formation_interfaces::msg::LeaderState::_phase_type arg)
  {
    msg_.phase = std::move(arg);
    return std::move(msg_);
  }

private:
  ::formation_interfaces::msg::LeaderState msg_;
};

class Init_LeaderState_yaw_rate
{
public:
  explicit Init_LeaderState_yaw_rate(::formation_interfaces::msg::LeaderState & msg)
  : msg_(msg)
  {}
  Init_LeaderState_phase yaw_rate(::formation_interfaces::msg::LeaderState::_yaw_rate_type arg)
  {
    msg_.yaw_rate = std::move(arg);
    return Init_LeaderState_phase(msg_);
  }

private:
  ::formation_interfaces::msg::LeaderState msg_;
};

class Init_LeaderState_yaw
{
public:
  explicit Init_LeaderState_yaw(::formation_interfaces::msg::LeaderState & msg)
  : msg_(msg)
  {}
  Init_LeaderState_yaw_rate yaw(::formation_interfaces::msg::LeaderState::_yaw_type arg)
  {
    msg_.yaw = std::move(arg);
    return Init_LeaderState_yaw_rate(msg_);
  }

private:
  ::formation_interfaces::msg::LeaderState msg_;
};

class Init_LeaderState_velocity
{
public:
  explicit Init_LeaderState_velocity(::formation_interfaces::msg::LeaderState & msg)
  : msg_(msg)
  {}
  Init_LeaderState_yaw velocity(::formation_interfaces::msg::LeaderState::_velocity_type arg)
  {
    msg_.velocity = std::move(arg);
    return Init_LeaderState_yaw(msg_);
  }

private:
  ::formation_interfaces::msg::LeaderState msg_;
};

class Init_LeaderState_position
{
public:
  explicit Init_LeaderState_position(::formation_interfaces::msg::LeaderState & msg)
  : msg_(msg)
  {}
  Init_LeaderState_velocity position(::formation_interfaces::msg::LeaderState::_position_type arg)
  {
    msg_.position = std::move(arg);
    return Init_LeaderState_velocity(msg_);
  }

private:
  ::formation_interfaces::msg::LeaderState msg_;
};

class Init_LeaderState_timestamp
{
public:
  Init_LeaderState_timestamp()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_LeaderState_position timestamp(::formation_interfaces::msg::LeaderState::_timestamp_type arg)
  {
    msg_.timestamp = std::move(arg);
    return Init_LeaderState_position(msg_);
  }

private:
  ::formation_interfaces::msg::LeaderState msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::formation_interfaces::msg::LeaderState>()
{
  return formation_interfaces::msg::builder::Init_LeaderState_timestamp();
}

}  // namespace formation_interfaces

#endif  // FORMATION_INTERFACES__MSG__DETAIL__LEADER_STATE__BUILDER_HPP_
