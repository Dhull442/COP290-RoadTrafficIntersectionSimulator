#ifndef ROAD_H
#define ROAD_H

#include <bits/stdc++.h>
#include "Vehicle.h"
#include "RenderEngine.h"

class Vehicle;

class Road {
        // All co-ordinates consider left bottom as (0,0)
    private:
        std::string signal; // The signal value at this time
        std::vector<std::pair<double, double> > map;
    public:
        // default vehicle Parameters
        #ifdef RENDER_ENGINE_H
        RenderEngine engine;
        #endif
        double default_maxspeed = 1;
        double default_acceleration = 1;
        double default_length = 2;
        double default_width = 2;
        int default_skill = 1;

        double length;
        double width;
        double signalPosition;
        int lanes;
        int id=-1;
        std::vector< int > signal_rgb;
        // Pointer to the Vehicle objects on the road
        std::vector<Vehicle*> vehicles;
        std::vector< std::pair< std::vector<Vehicle*> , double > > laneVehicles; // a pair of vehicles in lane and it's back end;
        // Initialize the Road object
        Road(int id, double length, double width);
        Road(int id);
        Road();
        void updateSim(double delT);
        void setDefaults(double maxspeed, double acceleration,double length, double width,int skill);
        // Add a Vehicle to the road
        void addVehicle(Vehicle* vehicle,std::string color);
        void updateUnrestrictedpositions(double delT);
        void addtoLanes(Vehicle* vehicle,int numlanesreq,int toplane);
        // First vehicle obstacle in a lane
        // double firstObstacle(double startPos,double length, double topRow, double botRow );
        double firstObstacle(Vehicle* vehicle);
        bool isRed();
        void initLanes(int lanes);
        std::pair<double,double> initPosition(Vehicle* vehicle);
        void error_callback(std::string errormsg);
        // Run the simulation on the road for time t
        void runSim(double t);
        void setSignal(std::string signal);
        double findLast(std::vector<Vehicle*> vehicles);
        void changeLane(Vehicle* vehicle);
        void updateLane(int a,Vehicle* b);
        void removeFromLane(int lane,Vehicle* v);
        void printLanes();
        bool hasSpace(std::vector<Vehicle*> Vehicles,double front,double back);
        // std::pair<double,double> initPosition(Vehicle* v);

    };

#endif
