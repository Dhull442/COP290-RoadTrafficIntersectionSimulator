#ifndef VEHICLE_H
#define VEHICLE_H

#include <bits/stdc++.h>
#include "Road.h"

class Road;

class Vehicle {
    private:
        double actualverticalspeed;

    public:
        std::string type;
        std::string color;
        double oldSafedistance;
        double velLimit;
        bool emergency;
        bool useLimit;
        double length, width, safedistance;
        double closestDistance;
        Vehicle* front;
        Vehicle* back;
        int skill;
        std::string ascii_color;
        double maxspeed;
        double acceleration, a;
        double currentSpeed;
        double theta;
        std::pair<double,double> currentPosition; // The coordinate of the front-top of the vehicle
        std::pair<double,double> unrestrictedposition;
        std::vector< int > color_rgb;
        bool isOnRoad;
        bool processed;
        double delT;
        bool isPossible(double delT);
        Road* parentRoad; // Pointer to the road on which the vehicle is
        std::pair<int,int> currentLane;
        bool stopped = false;
        // Checks to see if we are currently changing the lane
        bool changingLane;
        // Ratio of horizontal to vertical speed
        double speedRatio;
        // Last time lane change occured
        double lastLaneChange;
        // Time gap between two Lane changes
        double timeGap;
        // The vertical speed
        double verticalSpeed;
        // The direction of lane change (bottom->top is +1 top->bottom is -1)
        double changeDirection;
        void changeLane(double delT, double globalTime);
        // The unsigned vertical distance travelle during lane change
        double verticalPosition;
        // Initializes a Vehicle object with default values
        Vehicle();

        Vehicle(std::string type);
        // Intializes a Vehicle with values
        Vehicle(std::string type, double length, double width);
        void reConstruct();
        void setColor(std::string color);
        double activation_function(double speed);
        // Updates the position and velocity of the car based on delT
        void updatePos(double delT, double globalTime);
};

#endif
