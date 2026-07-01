// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from formation_interfaces:msg/LeaderState.idl
// generated code does not contain a copyright notice

#ifndef FORMATION_INTERFACES__MSG__DETAIL__LEADER_STATE__STRUCT_HPP_
#define FORMATION_INTERFACES__MSG__DETAIL__LEADER_STATE__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__formation_interfaces__msg__LeaderState __attribute__((deprecated))
#else
# define DEPRECATED__formation_interfaces__msg__LeaderState __declspec(deprecated)
#endif

namespace formation_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct LeaderState_
{
  using Type = LeaderState_<ContainerAllocator>;

  explicit LeaderState_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->timestamp = 0ull;
      std::fill<typename std::array<double, 3>::iterator, double>(this->position.begin(), this->position.end(), 0.0);
      std::fill<typename std::array<double, 3>::iterator, double>(this->velocity.begin(), this->velocity.end(), 0.0);
      this->yaw = 0.0;
      this->yaw_rate = 0.0;
      this->phase = 0;
    }
  }

  explicit LeaderState_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : position(_alloc),
    velocity(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->timestamp = 0ull;
      std::fill<typename std::array<double, 3>::iterator, double>(this->position.begin(), this->position.end(), 0.0);
      std::fill<typename std::array<double, 3>::iterator, double>(this->velocity.begin(), this->velocity.end(), 0.0);
      this->yaw = 0.0;
      this->yaw_rate = 0.0;
      this->phase = 0;
    }
  }

  // field types and members
  using _timestamp_type =
    uint64_t;
  _timestamp_type timestamp;
  using _position_type =
    std::array<double, 3>;
  _position_type position;
  using _velocity_type =
    std::array<double, 3>;
  _velocity_type velocity;
  using _yaw_type =
    double;
  _yaw_type yaw;
  using _yaw_rate_type =
    double;
  _yaw_rate_type yaw_rate;
  using _phase_type =
    uint8_t;
  _phase_type phase;

  // setters for named parameter idiom
  Type & set__timestamp(
    const uint64_t & _arg)
  {
    this->timestamp = _arg;
    return *this;
  }
  Type & set__position(
    const std::array<double, 3> & _arg)
  {
    this->position = _arg;
    return *this;
  }
  Type & set__velocity(
    const std::array<double, 3> & _arg)
  {
    this->velocity = _arg;
    return *this;
  }
  Type & set__yaw(
    const double & _arg)
  {
    this->yaw = _arg;
    return *this;
  }
  Type & set__yaw_rate(
    const double & _arg)
  {
    this->yaw_rate = _arg;
    return *this;
  }
  Type & set__phase(
    const uint8_t & _arg)
  {
    this->phase = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    formation_interfaces::msg::LeaderState_<ContainerAllocator> *;
  using ConstRawPtr =
    const formation_interfaces::msg::LeaderState_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<formation_interfaces::msg::LeaderState_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<formation_interfaces::msg::LeaderState_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      formation_interfaces::msg::LeaderState_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<formation_interfaces::msg::LeaderState_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      formation_interfaces::msg::LeaderState_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<formation_interfaces::msg::LeaderState_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<formation_interfaces::msg::LeaderState_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<formation_interfaces::msg::LeaderState_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__formation_interfaces__msg__LeaderState
    std::shared_ptr<formation_interfaces::msg::LeaderState_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__formation_interfaces__msg__LeaderState
    std::shared_ptr<formation_interfaces::msg::LeaderState_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const LeaderState_ & other) const
  {
    if (this->timestamp != other.timestamp) {
      return false;
    }
    if (this->position != other.position) {
      return false;
    }
    if (this->velocity != other.velocity) {
      return false;
    }
    if (this->yaw != other.yaw) {
      return false;
    }
    if (this->yaw_rate != other.yaw_rate) {
      return false;
    }
    if (this->phase != other.phase) {
      return false;
    }
    return true;
  }
  bool operator!=(const LeaderState_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct LeaderState_

// alias to use template instance with default allocator
using LeaderState =
  formation_interfaces::msg::LeaderState_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace formation_interfaces

#endif  // FORMATION_INTERFACES__MSG__DETAIL__LEADER_STATE__STRUCT_HPP_
