import setup_path
import airsim
import math
import time

# Connect to the AirSim simulator and specify the vehicle name
import argparse

# Parse command line argumentsvehic
parser = argparse.ArgumentParser(description='Command drone to hover')
parser.add_argument('--vehicle_name', type=str, default="SimpleFlight0",
                   help='Name of the vehicle to control')
parser.add_argument('--height', type=float, default=-3.0,
                   help='Hover height in meters (negative means above ground)')
parser.add_argument('--duration', type=int, default=1800,
                   help='Hover duration in seconds')
args = parser.parse_args()

time.sleep(5)
client = airsim.MultirotorClient()
client.confirmConnection()

# Specify the vehicle name "SimpleFlight0"
vehicle_name = args.vehicle_name
client.enableApiControl(True, vehicle_name)
client.armDisarm(True, vehicle_name)

# Takeoff
client.takeoffAsync(vehicle_name=vehicle_name).join()

client.moveToZAsync(args.height, 0.1).join()

print("Takeoff complete")

duration = args.duration  # 30 minutes = 1800 seconds
time.sleep(duration)


print("Landing")
client.landAsync(vehicle_name=vehicle_name).join()
# Disarm with specified vehicle name
client.armDisarm(False, vehicle_name=vehicle_name)