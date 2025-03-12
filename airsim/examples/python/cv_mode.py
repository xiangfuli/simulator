# In settings.json first activate computer vision mode:
# https://github.com/Microsoft/AirSim/blob/main/docs/image_apis.md#computer-vision-mode

import setup_path
import airsim

import pprint
import os
import time
import math
import tempfile

pp = pprint.PrettyPrinter(indent=4)

client = airsim.VehicleClient()
client.confirmConnection()

airsim.wait_key('Press any key to set camera-0 gimbal to 15-degree pitch')
camera_pose = airsim.Pose(airsim.Vector3r(0, 0, 0), airsim.to_quaternion(math.radians(15), 0, 0)) #radians
client.simSetCameraPose("0", camera_pose)

airsim.wait_key('Press any key to get camera parameters')
for camera_name in range(5):
    camera_info = client.simGetCameraInfo(str(camera_name))
    print("CameraInfo %d:" % camera_name)
    pp.pprint(camera_info)