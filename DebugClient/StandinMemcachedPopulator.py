from pymemcache.client.base import Client
import time
import numpy as np


def rotate_z(point, angle):
    x = point[0]
    y = point[1]
    rot_x = x*np.cos(angle) - y*np.sin(angle)
    rot_y = x*np.sin(angle) + y*np.cos(angle)
    rot_z = point[2]
    return (rot_x, rot_y, rot_z)


if __name__ == "__main__":

    # Connect to memcached process
    mc = Client(('localhost', 11211))

    magnetometer_reference = (0, 1, 1)
    while(1):
        now = time.time()
        now_mag = rotate_z(magnetometer_reference,now)
        mc.set("Simulation_Magnetometer_X", now_mag[0])
        mc.set("Simulation_Magnetometer_Y", now_mag[1])
        mc.set("Simulation_Magnetometer_Z", now_mag[2])

        print("Magnetometer data being fed in:")
        print(float(mc.get("Simulation_Magnetometer_X")))
        print(float(mc.get("Simulation_Magnetometer_Y")))
        print(float(mc.get("Simulation_Magnetometer_Z")))

        if mc.get("Simulation_Torque_X"):
            print("Torque output from flight computer:")
            print(float(mc.get("Simulation_Torque_X")))
            print(float(mc.get("Simulation_Torque_Y")))
            print(float(mc.get("Simulation_Torque_Z")))
        else:
            print("No torque input yet")

        time.sleep(0.5)
