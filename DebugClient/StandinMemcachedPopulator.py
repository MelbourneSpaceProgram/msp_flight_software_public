from pymemcache.client.base import Client
import time
import numpy as np
import struct


def rotate_z(point, angle):
    x = point[0]
    y = point[1]
    rot_x = x*np.cos(angle) - y*np.sin(angle)
    rot_y = x*np.sin(angle) + y*np.cos(angle)
    rot_z = point[2]
    return (rot_x, rot_y, rot_z)


if __name__ == "__main__":

    # Connect to memcached process
    mc = Client(('localhost', 11211), allow_unicode_keys=True)

    magnetometer_reference = (0, 1, 1)

    while(1):
        #mc.set(b"Simulation_Torque_X",
        #    struct.pack('>d',0.01))
        #mc.set(b"Simulation_Torque_Y",
        #    struct.pack('>d',0.02))
        #mc.set(b"Simulation_Torque_Z",
        #    struct.pack('>d',0.03))

        #mc.set("Simulation_Magnetometer_X",
        #    struct.pack('>d',1))
        #mc.set("Simulation_Magnetometer_Y",
        #    struct.pack('>d',2))
        #mc.set("Simulation_Magnetometer_Z",
        #    struct.pack('>d',3))

        if mc.get("Simulation_Magnetometer_X"):
            print("Magnetometer data being fed in:")
            print(struct.unpack('>d', mc.get("Simulation_Magnetometer_X"))[0])
            print(struct.unpack('>d', mc.get("Simulation_Magnetometer_Y"))[0])
            print(struct.unpack('>d', mc.get("Simulation_Magnetometer_Z"))[0])

        if mc.get("Simulation_Torque_X"):
            print("Torque output from flight computer:")
            print(struct.unpack('>d', mc.get("Simulation_Torque_X"))[0])
            print(struct.unpack('>d', mc.get("Simulation_Torque_Y"))[0])
            print(struct.unpack('>d', mc.get("Simulation_Torque_Z"))[0])

        time.sleep(0.1)
