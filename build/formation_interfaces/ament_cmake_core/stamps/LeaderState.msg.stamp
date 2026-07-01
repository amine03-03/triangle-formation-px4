# Virtual-leader broadcast. Everything a drone needs to compute its slot.
# All vectors in the shared WORLD NED frame (see spawn_offset handling).
uint64 timestamp        # us since epoch (node clock)
float64[3] position     # world NED [north, east, down] (m)
float64[3] velocity     # world NED (m/s)
float64 yaw             # rad, -pi..pi, heading (from North, NED convention)
float64 yaw_rate        # rad/s
uint8 phase             # 0=INIT 1=ASSEMBLE 2=ENGAGE 3=HOLD 4=LAND
