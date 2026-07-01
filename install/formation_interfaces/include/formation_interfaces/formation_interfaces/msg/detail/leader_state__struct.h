// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from formation_interfaces:msg/LeaderState.idl
// generated code does not contain a copyright notice

#ifndef FORMATION_INTERFACES__MSG__DETAIL__LEADER_STATE__STRUCT_H_
#define FORMATION_INTERFACES__MSG__DETAIL__LEADER_STATE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/LeaderState in the package formation_interfaces.
/**
  * Virtual-leader broadcast. Everything a drone needs to compute its slot.
  * All vectors in the shared WORLD NED frame (see spawn_offset handling).
 */
typedef struct formation_interfaces__msg__LeaderState
{
  /// us since epoch (node clock)
  uint64_t timestamp;
  /// world NED [north, east, down] (m)
  double position[3];
  /// world NED (m/s)
  double velocity[3];
  /// rad, -pi..pi, heading (from North, NED convention)
  double yaw;
  /// rad/s
  double yaw_rate;
  /// 0=INIT 1=ASSEMBLE 2=ENGAGE 3=HOLD 4=LAND
  uint8_t phase;
} formation_interfaces__msg__LeaderState;

// Struct for a sequence of formation_interfaces__msg__LeaderState.
typedef struct formation_interfaces__msg__LeaderState__Sequence
{
  formation_interfaces__msg__LeaderState * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} formation_interfaces__msg__LeaderState__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // FORMATION_INTERFACES__MSG__DETAIL__LEADER_STATE__STRUCT_H_
