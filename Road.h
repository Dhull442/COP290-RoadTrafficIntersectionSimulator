#ifndef ROAD_H
#define ROAD_H

#include <bits/stdc++.h>
#include "Vehicle.h"
#ifdef D3
#include "Render.h"
#else
#include "RenderEngine.h"
#endif

class Vehicle;

class Road {
        // All co-ordinates consider left bottom as (0,0)
    private:
        std::string signal; // The signal value at this time
        std::vector<std::pair<double, double> > map;
        std::vector<double> calculateBackEnds();
        void addtoLanes(Vehicle* vehicle,int numlanesreq,int toplane);
        double findLast(std::vector<Vehicle*> vehicles);
        void updateLane(int a,Vehicle* b);
        void removeFromLane(int lane,Vehicle* v);
        bool hasSpace(std::vector<Vehicle*> Vehicles,double front,double back);
    public:
        // default vehicle Parameters
        RenderEngine engine;
        // The clearance required on either side of the vehicle
        double sideClearance;
        double default_maxspeed = 1;
        double default_acceleration = 1;
        double default_length = 2;
        double default_width = 2;
        double default_safety_distance = 1;
        double default_timegap;
        double default_speedratio;
        int default_skill = 1;
        double length;
        double width;
        double signalPosition;
        std::string ascii_signalcolor;
        int lanes;
        int id=-1;
        bool getAdjVehicles(Vehicle* vehicle, int dir, double delT, double globalTime);
        std::vector< int > signal_rgb;
        // Pointer to the Vehicle objects on the road
        std::vector<Vehicle*> vehicles;
        // Pointer to vehicles in Lanes
        std::vector< std::vector<Vehicle*> > laneVehicles;
        // Initialize the Road object
        Road(int id, double length, double width);
        Road(int id);
        Road();
        // Update the simulation in a step of delT
        void updateSim(double delT, double globalTime);
        void setDefaults(double maxspeed, double acceleration,double length, double width,int skill, double sdistance, double ratio, double timegap, double s);
        // Add a Vehicle to the road
        void addVehicle(Vehicle* vehicle,std::string color);
        // First vehicle obstacle in a lane
        // double firstObstacle(double startPos,double length, double topRow, double botRow );
        double firstObstacle(Vehicle* vehicle, double delT, double globalTime);
        void initLanes(int lanes);
        std::pair<double,double> initPosition(Vehicle* vehicle);
        void error_callback(std::string errormsg);
        void changeLane(Vehicle* vehicle);
        // Run the simulation on the road for time t
        void runSim(double t);
        void setSignal(std::string signal);
        void printLanes();
        bool isRed();
        void removeFromLane(Vehicle* v, int laneno);
        void insertInLane(Vehicle* front, int laneno, Vehicle* v);
    };

#endif
