#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



// Corresponds to formation_interfaces__msg__LeaderState
/// Virtual-leader broadcast. Everything a drone needs to compute its slot.
/// All vectors in the shared WORLD NED frame (see spawn_offset handling).

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
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
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::LeaderState::default())
  }
}

impl rosidl_runtime_rs::Message for LeaderState {
  type RmwMsg = super::msg::rmw::LeaderState;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        timestamp: msg.timestamp,
        position: msg.position,
        velocity: msg.velocity,
        yaw: msg.yaw,
        yaw_rate: msg.yaw_rate,
        phase: msg.phase,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      timestamp: msg.timestamp,
        position: msg.position,
        velocity: msg.velocity,
      yaw: msg.yaw,
      yaw_rate: msg.yaw_rate,
      phase: msg.phase,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      timestamp: msg.timestamp,
      position: msg.position,
      velocity: msg.velocity,
      yaw: msg.yaw,
      yaw_rate: msg.yaw_rate,
      phase: msg.phase,
    }
  }
}


// Corresponds to formation_interfaces__msg__DroneState
/// Per-drone state, published by every drone_interface, consumed by peers
/// (for the consensus term) and by the coordinator (for convergence checks).

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
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
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::DroneState::default())
  }
}

impl rosidl_runtime_rs::Message for DroneState {
  type RmwMsg = super::msg::rmw::DroneState;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        timestamp: msg.timestamp,
        index: msg.index,
        position: msg.position,
        yaw: msg.yaw,
        valid: msg.valid,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      timestamp: msg.timestamp,
      index: msg.index,
        position: msg.position,
      yaw: msg.yaw,
      valid: msg.valid,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      timestamp: msg.timestamp,
      index: msg.index,
      position: msg.position,
      yaw: msg.yaw,
      valid: msg.valid,
    }
  }
}


