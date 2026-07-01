#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};


#[link(name = "formation_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__formation_interfaces__msg__LeaderState() -> *const std::ffi::c_void;
}

#[link(name = "formation_interfaces__rosidl_generator_c")]
extern "C" {
    fn formation_interfaces__msg__LeaderState__init(msg: *mut LeaderState) -> bool;
    fn formation_interfaces__msg__LeaderState__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<LeaderState>, size: usize) -> bool;
    fn formation_interfaces__msg__LeaderState__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<LeaderState>);
    fn formation_interfaces__msg__LeaderState__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<LeaderState>, out_seq: *mut rosidl_runtime_rs::Sequence<LeaderState>) -> bool;
}

// Corresponds to formation_interfaces__msg__LeaderState
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// Virtual-leader broadcast. Everything a drone needs to compute its slot.
/// All vectors in the shared WORLD NED frame (see spawn_offset handling).

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct LeaderState {
    /// us since epoch (node clock)
    pub timestamp: u64,

    /// world NED [north, east, down] (m)
    pub position: [f64; 3],

    /// world NED (m/s)
    pub velocity: [f64; 3],

    /// rad, -pi..pi, heading (from North, NED convention)
    pub yaw: f64,

    /// rad/s
    pub yaw_rate: f64,

    /// 0=INIT 1=ASSEMBLE 2=ENGAGE 3=HOLD 4=LAND
    pub phase: u8,

}



impl Default for LeaderState {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !formation_interfaces__msg__LeaderState__init(&mut msg as *mut _) {
        panic!("Call to formation_interfaces__msg__LeaderState__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for LeaderState {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { formation_interfaces__msg__LeaderState__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { formation_interfaces__msg__LeaderState__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { formation_interfaces__msg__LeaderState__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for LeaderState {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for LeaderState where Self: Sized {
  const TYPE_NAME: &'static str = "formation_interfaces/msg/LeaderState";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__formation_interfaces__msg__LeaderState() }
  }
}


#[link(name = "formation_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__formation_interfaces__msg__DroneState() -> *const std::ffi::c_void;
}

#[link(name = "formation_interfaces__rosidl_generator_c")]
extern "C" {
    fn formation_interfaces__msg__DroneState__init(msg: *mut DroneState) -> bool;
    fn formation_interfaces__msg__DroneState__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<DroneState>, size: usize) -> bool;
    fn formation_interfaces__msg__DroneState__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<DroneState>);
    fn formation_interfaces__msg__DroneState__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<DroneState>, out_seq: *mut rosidl_runtime_rs::Sequence<DroneState>) -> bool;
}

// Corresponds to formation_interfaces__msg__DroneState
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// Per-drone state, published by every drone_interface, consumed by peers
/// (for the consensus term) and by the coordinator (for convergence checks).

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct DroneState {
    /// us
    pub timestamp: u64,

    /// formation slot index (0=apex, 1=rear-right, 2=rear-left)
    pub index: i32,

    /// world NED (m)  = local NED + spawn_offset
    pub position: [f64; 3],

    /// rad
    pub yaw: f64,

    /// true if position estimate is usable
    pub valid: bool,

}



impl Default for DroneState {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !formation_interfaces__msg__DroneState__init(&mut msg as *mut _) {
        panic!("Call to formation_interfaces__msg__DroneState__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for DroneState {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { formation_interfaces__msg__DroneState__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { formation_interfaces__msg__DroneState__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { formation_interfaces__msg__DroneState__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for DroneState {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for DroneState where Self: Sized {
  const TYPE_NAME: &'static str = "formation_interfaces/msg/DroneState";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__formation_interfaces__msg__DroneState() }
  }
}


