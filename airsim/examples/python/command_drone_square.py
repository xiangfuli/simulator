import setup_path
import airsim
import math
import time

# Connect to the AirSim simulator and specify the vehicle name
client = airsim.MultirotorClient()
client.confirmConnection()

# Specify the vehicle name "SimpleFlight0"
vehicle_name = "SimpleFlight0"
client.enableApiControl(True, vehicle_name)
client.armDisarm(True, vehicle_name)

# Takeoff
client.takeoffAsync(vehicle_name=vehicle_name).join()

print("Takeoff complete")

# Parameters for circular motion
radius = 5.0  # radius of circle in meters
height = -3.0  # negative means above ground
speed = 3.0  # speed in m/s
center = airsim.Vector3r(0, 0, height)  # center point of circle

# Calculate time needed for one revolution
circumference = 2 * math.pi * radius
revolution_time = circumference / speed

start_time = time.time()
duration = 3600  # fly for 30 seconds

square_points = [
    airsim.Vector3r(10, 10, height),
    airsim.Vector3r(-10, 10, height),
    airsim.Vector3r(-10, -10, height),
    airsim.Vector3r(10, -10, height),
]

square_index = 0
while time.time() - start_time < duration: # 30 circless
    # Calculate current angle based on time
    # Use constant angular velocity for smoother motion
    # Move drone to the position
    client.moveToPositionAsync(square_points[square_index].x_val, square_points[square_index].y_val, square_points[square_index].z_val, speed, vehicle_name=vehicle_name).join()

    square_index = square_index + 1
    square_index = square_index % len(square_points)
    print("Moving to point. x: %s, y: %s, z: %s" % (square_points[square_index].x_val, square_points[square_index].y_val, square_points[square_index].z_val))

# Land with specified vehicle name
client.landAsync(vehicle_name=vehicle_name).join()

# Disarm with specified vehicle name
client.armDisarm(False, vehicle_name=vehicle_name)