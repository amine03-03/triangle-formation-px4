// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from formation_interfaces:msg/DroneState.idl
// generated code does not contain a copyright notice
#include "formation_interfaces/msg/detail/drone_state__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
formation_interfaces__msg__DroneState__init(formation_interfaces__msg__DroneState * msg)
{
  if (!msg) {
    return false;
  }
  // timestamp
  // index
  // position
  // yaw
  // valid
  return true;
}

void
formation_interfaces__msg__DroneState__fini(formation_interfaces__msg__DroneState * msg)
{
  if (!msg) {
    return;
  }
  // timestamp
  // index
  // position
  // yaw
  // valid
}

bool
formation_interfaces__msg__DroneState__are_equal(const formation_interfaces__msg__DroneState * lhs, const formation_interfaces__msg__DroneState * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // timestamp
  if (lhs->timestamp != rhs->timestamp) {
    return false;
  }
  // index
  if (lhs->index != rhs->index) {
    return false;
  }
  // position
  for (size_t i = 0; i < 3; ++i) {
    if (lhs->position[i] != rhs->position[i]) {
      return false;
    }
  }
  // yaw
  if (lhs->yaw != rhs->yaw) {
    return false;
  }
  // valid
  if (lhs->valid != rhs->valid) {
    return false;
  }
  return true;
}

bool
formation_interfaces__msg__DroneState__copy(
  const formation_interfaces__msg__DroneState * input,
  formation_interfaces__msg__DroneState * output)
{
  if (!input || !output) {
    return false;
  }
  // timestamp
  output->timestamp = input->timestamp;
  // index
  output->index = input->index;
  // position
  for (size_t i = 0; i < 3; ++i) {
    output->position[i] = input->position[i];
  }
  // yaw
  output->yaw = input->yaw;
  // valid
  output->valid = input->valid;
  return true;
}

formation_interfaces__msg__DroneState *
formation_interfaces__msg__DroneState__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  formation_interfaces__msg__DroneState * msg = (formation_interfaces__msg__DroneState *)allocator.allocate(sizeof(formation_interfaces__msg__DroneState), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(formation_interfaces__msg__DroneState));
  bool success = formation_interfaces__msg__DroneState__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
formation_interfaces__msg__DroneState__destroy(formation_interfaces__msg__DroneState * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    formation_interfaces__msg__DroneState__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
formation_interfaces__msg__DroneState__Sequence__init(formation_interfaces__msg__DroneState__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  formation_interfaces__msg__DroneState * data = NULL;

  if (size) {
    data = (formation_interfaces__msg__DroneState *)allocator.zero_allocate(size, sizeof(formation_interfaces__msg__DroneState), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = formation_interfaces__msg__DroneState__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        formation_interfaces__msg__DroneState__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
formation_interfaces__msg__DroneState__Sequence__fini(formation_interfaces__msg__DroneState__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      formation_interfaces__msg__DroneState__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

formation_interfaces__msg__DroneState__Sequence *
formation_interfaces__msg__DroneState__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  formation_interfaces__msg__DroneState__Sequence * array = (formation_interfaces__msg__DroneState__Sequence *)allocator.allocate(sizeof(formation_interfaces__msg__DroneState__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = formation_interfaces__msg__DroneState__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
formation_interfaces__msg__DroneState__Sequence__destroy(formation_interfaces__msg__DroneState__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    formation_interfaces__msg__DroneState__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
formation_interfaces__msg__DroneState__Sequence__are_equal(const formation_interfaces__msg__DroneState__Sequence * lhs, const formation_interfaces__msg__DroneState__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!formation_interfaces__msg__DroneState__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
formation_interfaces__msg__DroneState__Sequence__copy(
  const formation_interfaces__msg__DroneState__Sequence * input,
  formation_interfaces__msg__DroneState__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(formation_interfaces__msg__DroneState);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    formation_interfaces__msg__DroneState * data =
      (formation_interfaces__msg__DroneState *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!formation_interfaces__msg__DroneState__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          formation_interfaces__msg__DroneState__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!formation_interfaces__msg__DroneState__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
