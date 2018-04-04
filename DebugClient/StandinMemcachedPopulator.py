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
        #    struct.pack('>d',1e-5))
        #mc.set(b"Simulation_Torque_Y",
        #    struct.pack('>d',2e-5))
        #mc.set(b"Simulation_Torque_Z",
        #    struct.pack('>d',3e-5))

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

        if mc.get("Simulation_IR_X_Pos"):
            print("Infrared output from simulation:")
            print(struct.unpack('>d', mc.get("Simulation_IR_X_Pos"))[0])
            print(struct.unpack('>d', mc.get("Simulation_IR_X_Neg"))[0])
            print(struct.unpack('>d', mc.get("Simulation_IR_Y_Pos"))[0])
            print(struct.unpack('>d', mc.get("Simulation_IR_Y_Neg"))[0])
            print(struct.unpack('>d', mc.get("Simulation_IR_Z_Pos"))[0])
            print(struct.unpack('>d', mc.get("Simulation_IR_Z_Neg"))[0])

        if mc.get("Simulation_TLE_Mean_Anomaly"):
            print("TLE output from simulation:")
            print(struct.unpack('>d', mc.get("Simulation_TLE_Mean_Anomaly"))[0])
            print(struct.unpack('>d', mc.get("Simulation_TLE_Mean_Motion"))[0])
            print(struct.unpack('>d', mc.get("Simulation_TLE_Mean_Motion_First_Deriv"))[0])
            print(struct.unpack('>d', mc.get("Simulation_TLE_Mean_Motion_Second_Deriv"))[0])
            print(struct.unpack('>d', mc.get("Simulation_TLE_Argument_Perigee"))[0])
            print(struct.unpack('>d', mc.get("Simulation_TLE_Eccentricity_1e7"))[0])
            print(struct.unpack('>d', mc.get("Simulation_TLE_Inclination"))[0])
            print(struct.unpack('>d', mc.get("Simulation_TLE_Bstar"))[0])
            print(struct.unpack('>d', mc.get("Simulation_TLE_Epoch"))[0])
            print(struct.unpack('>d', mc.get("Simulation_TLE_Raan"))[0])

        if mc.get("Location_Lattitude_Geodetic_Degrees"):
            print("Location output from flight computer:")
            print(struct.unpack('>d', mc.get("Location_Lattitude_Geodetic_Degrees"))[0])
            print(struct.unpack('>d', mc.get("Location_Longitude_Degrees"))[0])
            print(struct.unpack('>d', mc.get("Location_Altitude_Above_Ellipsoid_Km"))[0])

        time.sleep(0.05)
