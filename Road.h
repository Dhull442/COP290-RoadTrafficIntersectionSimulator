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
        // The initial position of the queue.
        // double queuePos;
        // The default distance between two objects.
        // double bufferLength;
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
        int window_length;
        int window_height;
        int id=-1;
        // GLFWwindow* window; // The rendering window
        std::vector< int > signal_rgb;
        // Pointer to the Vehicle objects on the road
        std::vector<Vehicle*> vehicles;
        // Initialize the Road object
        Road(int id, double length, double width);
        Road(int id);
        Road();
        void updateSim(double delT);
        void setDefaults(double maxspeed, double acceleration,double length, double width,int skill);
        // Add a Vehicle to the road
        void addVehicle(Vehicle* vehicle,std::string color);
        void updateUnrestrictedpositions(double delT);

        // First vehicle obstacle in a lane
        double firstObstacle(double startPos,double length, double topRow, double botRow );
        bool isRed();
        // Run the simulation on the road for time t
        void runSim(double t);
        void setSignal(std::string signal);
        // The error_callback function
        // static void error_callback(int error, const char* description);
        // The key function
        // static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        std::pair<double,double> initPosition();
        // std::vector<std::pair<double ,double> > topedgeprofile();
        // std::vector<std::pair<double ,double> > botedgeprofile();
        // Setup the OpenGL context with the road
        // void setupRoad();

        // Render the road into the window, reading the class attributes
        // void renderRoad();
    };

#endif
