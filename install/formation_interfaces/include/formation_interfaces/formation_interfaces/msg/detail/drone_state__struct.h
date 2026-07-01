// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from formation_interfaces:msg/DroneState.idl
// generated code does not contain a copyright notice

#ifndef FORMATION_INTERFACES__MSG__DETAIL__DRONE_STATE__STRUCT_H_
#define FORMATION_INTERFACES__MSG__DETAIL__DRONE_STATE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/DroneState in the package formation_interfaces.
/**
  * Per-drone state, published by every drone_interface, consumed by peers
  * (for the consensus term) and by the coordinator (for convergence checks).
 */
typedef struct formation_interfaces__msg__DroneState
{
  /// us
  uint64_t timestamp;
  /// formation slot index (0=apex, 1=rear-right, 2=rear-left)
  int32_t index;
  /// world NED (m)  = local NED + spawn_offset
  double position[3];
  /// rad
  double yaw;
  /// true if position estimate is usable
  bool valid;
} formation_interfaces__msg__DroneState;

// Struct for a sequence of formation_interfaces__msg__DroneState.
typedef struct formation_interfaces__msg__DroneState__Sequence
{
  formation_interfaces__msg__DroneState * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} formation_interfaces__msg__DroneState__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // FORMATION_INTERFACES__MSG__DETAIL__DRONE_STATE__STRUCT_H_
