#ifndef VEHICLE_H
#define VEHICLE_H

#include <bits/stdc++.h>
// #include "Road.h"
// #include <GLFW/glfw3.h>
// #include <stdlib.h>
// #include <stdio.h>

class Vehicle {
    private:


    public:
        std::string type;
        double length, width;
        int skill;
        double maxspeed;
        double acceleration;
        double currentSpeed;
        std::string color;
        std::pair<double,double> currentPosition; // The coordinate of the front-top of the vehicle
        std::pair<double,double> unrestrictedposition;
        bool isOnRoad;
        Road* parentRoad; // Pointer to the road on which the vehicle is


        // Initializes a Vehicle object with default values
        Vehicle();

        Vehicle(std::string type);
        // Intializes a Vehicle with values
        Vehicle(std::string type, double length, double width);
        reConstruct();
        setColor(std::string color);

        // Updates the position and velocity of the car based on delT
        void updatePos(double delT);
};

#endif
