#ifndef VEHICLE_H
#define VEHICLE_H

#include <bits/stdc++.h>

class Vehicle {
    private:
        int default_maxspeed = 1;
        int default_acceleration  1;
        int default_length = 2;
        int default_width = 2;

    public:
        std::string type;
        int length, width;
        int maxspeed;
        int acceleration;
        int currentSpeed;
        int currentPos; // The coordinate of the front of the vehicle
        bool isOnRoad;

        // Initializes a Vehicle object with default values
        Vehicle(std::string type, int length, int width) {
            self.type = type;
            self.length = default_length;
            self.width = default_width;
            self.maxspeed = default_maxspeed;
            self.acceleration = default_acceleration;
            // Initially the vehicle is not on the road so some parameters are undefined
            self.currentSpeed = -1;
            self.currentPos = -1;
            self.isOnRoad = false;
        }

}

#endif
