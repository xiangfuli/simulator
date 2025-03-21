import setup_path
import airsim
import math
import time
import argparse

def parse_args():
    parser = argparse.ArgumentParser(description='Control drone movement through specified points')
    parser.add_argument('--points', nargs='+', type=float, default=[-10,10, -10,-10],
                        help='List of points as x1 y1 x2 y2 x3 y3 ... (default: [-10,10, -10,-10])')
    parser.add_argument('--height', type=float, default=-3.0,
                        help='Flight height (negative means above ground) (default: -3.0)')
    parser.add_argument('--speed', type=float, default=3.0,
                        help='Flight speed in m/s (default: 3.0)')
    parser.add_argument('--duration', type=int, default=3600,
                        help='Flight duration in seconds (default: 3600)')
    return parser.parse_args()

def create_trajectory_points(point_list, height):
    # Convert flat list of coordinates into Vector3r points
    points = []
    for i in range(0, len(point_list), 2):
        if i + 1 < len(point_list):
            points.append(airsim.Vector3r(point_list[i], point_list[i+1], height))
    return points

def main():
    args = parse_args()
    
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

    # Create trajectory points from input arguments
    trajectory_points = create_trajectory_points(args.points, args.height)
    
    if len(trajectory_points) == 0:
        print("Error: No valid points provided")
        return

    print(f"Flying through {len(trajectory_points)} points at height {args.height}m with speed {args.speed}m/s")
    
    start_time = time.time()
    point_index = 0
    
    while time.time() - start_time < args.duration:
        current_point = trajectory_points[point_index]
        print(f"Moving to point {point_index + 1}: x={current_point.x_val}, y={current_point.y_val}, z={current_point.z_val}")
        
        # Move drone to the position
        client.moveToPositionAsync(
            current_point.x_val, 
            current_point.y_val, 
            current_point.z_val, 
            args.speed, 
            vehicle_name=vehicle_name
        ).join()

        # Move to next point (loop back to start if at end)
        point_index = (point_index + 1) % len(trajectory_points)

    # Land with specified vehicle name
    client.landAsync(vehicle_name=vehicle_name).join()

    # Disarm with specified vehicle name
    client.armDisarm(False, vehicle_name=vehicle_name)

if __name__ == "__main__":
    main()