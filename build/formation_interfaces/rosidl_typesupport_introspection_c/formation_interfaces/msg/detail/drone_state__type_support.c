// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from formation_interfaces:msg/DroneState.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "formation_interfaces/msg/detail/drone_state__rosidl_typesupport_introspection_c.h"
#include "formation_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "formation_interfaces/msg/detail/drone_state__functions.h"
#include "formation_interfaces/msg/detail/drone_state__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void formation_interfaces__msg__DroneState__rosidl_typesupport_introspection_c__DroneState_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  formation_interfaces__msg__DroneState__init(message_memory);
}

void formation_interfaces__msg__DroneState__rosidl_typesupport_introspection_c__DroneState_fini_function(void * message_memory)
{
  formation_interfaces__msg__DroneState__fini(message_memory);
}

size_t formation_interfaces__msg__DroneState__rosidl_typesupport_introspection_c__size_function__DroneState__position(
  const void * untyped_member)
{
  (void)untyped_member;
  return 3;
}

const void * formation_interfaces__msg__DroneState__rosidl_typesupport_introspection_c__get_const_function__DroneState__position(
  const void * untyped_member, size_t index)
{
  const double * member =
    (const double *)(untyped_member);
  return &member[index];
}

void * formation_interfaces__msg__DroneState__rosidl_typesupport_introspection_c__get_function__DroneState__position(
  void * untyped_member, size_t index)
{
  double * member =
    (double *)(untyped_member);
  return &member[index];
}

void formation_interfaces__msg__DroneState__rosidl_typesupport_introspection_c__fetch_function__DroneState__position(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const double * item =
    ((const double *)
    formation_interfaces__msg__DroneState__rosidl_typesupport_introspection_c__get_const_function__DroneState__position(untyped_member, index));
  double * value =
    (double *)(untyped_value);
  *value = *item;
}

void formation_interfaces__msg__DroneState__rosidl_typesupport_introspection_c__assign_function__DroneState__position(
  void * untyped_member, size_t index, const void * untyped_value)
{
  double * item =
    ((double *)
    formation_interfaces__msg__DroneState__rosidl_typesupport_introspection_c__get_function__DroneState__position(untyped_member, index));
  const double * value =
    (const double *)(untyped_value);
  *item = *value;
}

static rosidl_typesupport_introspection_c__MessageMember formation_interfaces__msg__DroneState__rosidl_typesupport_introspection_c__DroneState_message_member_array[5] = {
  {
    "timestamp",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT64,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(formation_interfaces__msg__DroneState, timestamp),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "index",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(formation_interfaces__msg__DroneState, index),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "position",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    3,  // array size
    false,  // is upper bound
    offsetof(formation_interfaces__msg__DroneState, position),  // bytes offset in struct
    NULL,  // default value
    formation_interfaces__msg__DroneState__rosidl_typesupport_introspection_c__size_function__DroneState__position,  // size() function pointer
    formation_interfaces__msg__DroneState__rosidl_typesupport_introspection_c__get_const_function__DroneState__position,  // get_const(index) function pointer
    formation_interfaces__msg__DroneState__rosidl_typesupport_introspection_c__get_function__DroneState__position,  // get(index) function pointer
    formation_interfaces__msg__DroneState__rosidl_typesupport_introspection_c__fetch_function__DroneState__position,  // fetch(index, &value) function pointer
    formation_interfaces__msg__DroneState__rosidl_typesupport_introspection_c__assign_function__DroneState__position,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "yaw",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(formation_interfaces__msg__DroneState, yaw),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "valid",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(formation_interfaces__msg__DroneState, valid),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers formation_interfaces__msg__DroneState__rosidl_typesupport_introspection_c__DroneState_message_members = {
  "formation_interfaces__msg",  // message namespace
  "DroneState",  // message name
  5,  // number of fields
  sizeof(formation_interfaces__msg__DroneState),
  formation_interfaces__msg__DroneState__rosidl_typesupport_introspection_c__DroneState_message_member_array,  // message members
  formation_interfaces__msg__DroneState__rosidl_typesupport_introspection_c__DroneState_init_function,  // function to initialize message memory (memory has to be allocated)
  formation_interfaces__msg__DroneState__rosidl_typesupport_introspection_c__DroneState_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t formation_interfaces__msg__DroneState__rosidl_typesupport_introspection_c__DroneState_message_type_support_handle = {
  0,
  &formation_interfaces__msg__DroneState__rosidl_typesupport_introspection_c__DroneState_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_formation_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, formation_interfaces, msg, DroneState)() {
  if (!formation_interfaces__msg__DroneState__rosidl_typesupport_introspection_c__DroneState_message_type_support_handle.typesupport_identifier) {
    formation_interfaces__msg__DroneState__rosidl_typesupport_introspection_c__DroneState_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &formation_interfaces__msg__DroneState__rosidl_typesupport_introspection_c__DroneState_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
