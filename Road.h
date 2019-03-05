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
        float queuePos;
        // The default distance between two objects.
        float bufferLength;
    public:
        float length;
        float width;
        int window_length;
        int window_height;
        int id;
        GLFWwindow* window; // The rendering window
        bool isClear; // Checks if the road is clear
        std::string signal; // The signal value at this time

        // Pointer to the Vehicle objects on the road
        std::vector<Vehicle*> vehicles;

        // Initialize the Road object
        Road(int id, float length, float width, float delT = 0.1);

        // Add a Vehicle to the road
        void addVehicle(Vehicle* vehicle);

        // Run the simulation on the road for time t
        void runSim(float t);

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
