# Per-drone state, published by every drone_interface, consumed by peers
# (for the consensus term) and by the coordinator (for convergence checks).
uint64 timestamp        # us
int32 index             # formation slot index (0=apex, 1=rear-right, 2=rear-left)
float64[3] position     # world NED (m)  = local NED + spawn_offset
float64 yaw             # rad
bool valid              # true if position estimate is usable
