// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from formation_interfaces:msg/LeaderState.idl
// generated code does not contain a copyright notice

#ifndef FORMATION_INTERFACES__MSG__DETAIL__LEADER_STATE__TRAITS_HPP_
#define FORMATION_INTERFACES__MSG__DETAIL__LEADER_STATE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "formation_interfaces/msg/detail/leader_state__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace formation_interfaces
{

namespace msg
{

inline void to_flow_style_yaml(
  const LeaderState & msg,
  std::ostream & out)
{
  out << "{";
  // member: timestamp
  {
    out << "timestamp: ";
    rosidl_generator_traits::value_to_yaml(msg.timestamp, out);
    out << ", ";
  }

  // member: position
  {
    if (msg.position.size() == 0) {
      out << "position: []";
    } else {
      out << "position: [";
      size_t pending_items = msg.position.size();
      for (auto item : msg.position) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: velocity
  {
    if (msg.velocity.size() == 0) {
      out << "velocity: []";
    } else {
      out << "velocity: [";
      size_t pending_items = msg.velocity.size();
      for (auto item : msg.velocity) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: yaw
  {
    out << "yaw: ";
    rosidl_generator_traits::value_to_yaml(msg.yaw, out);
    out << ", ";
  }

  // member: yaw_rate
  {
    out << "yaw_rate: ";
    rosidl_generator_traits::value_to_yaml(msg.yaw_rate, out);
    out << ", ";
  }

  // member: phase
  {
    out << "phase: ";
    rosidl_generator_traits::value_to_yaml(msg.phase, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const LeaderState & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: timestamp
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "timestamp: ";
    rosidl_generator_traits::value_to_yaml(msg.timestamp, out);
    out << "\n";
  }

  // member: position
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.position.size() == 0) {
      out << "position: []\n";
    } else {
      out << "position:\n";
      for (auto item : msg.position) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: velocity
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.velocity.size() == 0) {
      out << "velocity: []\n";
    } else {
      out << "velocity:\n";
      for (auto item : msg.velocity) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: yaw
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "yaw: ";
    rosidl_generator_traits::value_to_yaml(msg.yaw, out);
    out << "\n";
  }

  // member: yaw_rate
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "yaw_rate: ";
    rosidl_generator_traits::value_to_yaml(msg.yaw_rate, out);
    out << "\n";
  }

  // member: phase
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "phase: ";
    rosidl_generator_traits::value_to_yaml(msg.phase, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const LeaderState & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace formation_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use formation_interfaces::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const formation_interfaces::msg::LeaderState & msg,
  std::ostream & out, size_t indentation = 0)
{
  formation_interfaces::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use formation_interfaces::msg::to_yaml() instead")]]
inline std::string to_yaml(const formation_interfaces::msg::LeaderState & msg)
{
  return formation_interfaces::msg::to_yaml(msg);
}

template<>
inline const char * data_type<formation_interfaces::msg::LeaderState>()
{
  return "formation_interfaces::msg::LeaderState";
}

template<>
inline const char * name<formation_interfaces::msg::LeaderState>()
{
  return "formation_interfaces/msg/LeaderState";
}

template<>
struct has_fixed_size<formation_interfaces::msg::LeaderState>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<formation_interfaces::msg::LeaderState>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<formation_interfaces::msg::LeaderState>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // FORMATION_INTERFACES__MSG__DETAIL__LEADER_STATE__TRAITS_HPP_
