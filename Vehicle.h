#ifndef VEHICLE_H
#define VEHICLE_H

#include <bits/stdc++.h>
#include "Road.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

class Vehicle {
    private:
        float default_maxspeed = 1;
        float default_acceleration  1;
        float default_length = 2;
        float default_width = 2;

    public:
        std::string type;
        float length, width;
        float maxspeed;
        float acceleration;
        float currentSpeed;
        float currentPos; // The coordinate of the front of the vehicle
        bool isOnRoad;
        Road* parentRoad; // Pointer to the road on which the vehicle is

        // Initializes a Vehicle object with default values
        Vehicle(std::string type, float length, float width);

        // Updates the position and velocity of the car based on delT
        void updatePos(float delT);
}

#endif
