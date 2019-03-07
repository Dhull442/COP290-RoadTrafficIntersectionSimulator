#ifndef ROAD_H
#define ROAD_H

#include <bits/stdc++.h>
#include "Vehicle.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

class Road {
    private:
        // The initial position of the queue.
        double queuePos;
        // The default distance between two objects.
        double bufferLength;
    public:
        // default vehicle Parameters
        std::string default_type = "not specified";
        double default_maxspeed = 1;
        double default_acceleration = 1;
        double default_length = 2;
        double default_width = 2;
        int default_skill = 1;
        double speed_limit = -1;


        double length;
        double width;
        double signalPosition;
        int window_length;
        int window_height;
        int id;
        GLFWwindow* window; // The rendering window
        bool isClear; // Checks if the road is clear
        std::string signal; // The signal value at this time

        // Pointer to the Vehicle objects on the road
        std::vector<Vehicle*> vehicles;

        // Initialize the Road object
        Road(int id, double length, double width, double delT = 0.1);
        Road(int id);
        Road();
        void setDefaults(double maxspeed, double acceleration,double length, double width,int skill,double speed_limit)
        // Add a Vehicle to the road
        void addVehicle(Vehicle* vehicle);

        void updateUnrestrictedpositions(double delT);

        // First vehicle obstacle in a lane
        double firstObstacle(double startPos, double topRow, double botRow )

        // Run the simulation on the road for time t
        void runSim(double t);

        // The error_callback function
        static void error_callback(int error, const char* description);
        // The key function
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

        // Setup the OpenGL context with the road
        void setupRoad();

        // Render the road into the window, reading the class attributes
        void renderRoad();
    }

#endif
