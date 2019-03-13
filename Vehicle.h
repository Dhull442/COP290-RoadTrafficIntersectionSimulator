#ifndef VEHICLE_H
#define VEHICLE_H

#include <bits/stdc++.h>
#include "Road.h"

class Road;

class Vehicle {
    private:
        std::string color;

    public:
        std::string type;
        double length, width;
        int skill;
        double maxspeed;
        double acceleration;
        double currentSpeed;
        std::pair<double,double> currentPosition; // The coordinate of the front-top of the vehicle
        std::pair<double,double> unrestrictedposition;
        std::vector< int > color_rgb;
        bool isOnRoad;
        bool processed;
        double delT;
        Road* parentRoad; // Pointer to the road on which the vehicle is


        // Initializes a Vehicle object with default values
        Vehicle();

        Vehicle(std::string type);
        // Intializes a Vehicle with values
        Vehicle(std::string type, double length, double width);
        void reConstruct();
        void setColor(std::string color);

        // Updates the position and velocity of the car based on delT
        void updatePos(bool limit);
};

#endif
