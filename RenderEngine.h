#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include <bits/stdc++.h>
#include "Vehicle.h"
#include "Road.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

// This class takes a Road object and renders it
class RenderEngine {
  public:
    // The road that this will render
    Road* targetRoad;
    // The scaling factor
    int scaling;
    // Used to create a full screen simulation
    int monitorWidth, monitorHeight;
    // Set the background color
    vector<float> bgcolor;

    // The variable which store the OpenGL window
    GLFWwindow* window;
    // Constructor function
    RenderEngine(Road* targetRoad, int scaling=20);

    // The error callback function
    static void error_callback(int error, const char* description);
    // The key_callback function
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    // Initialize the variables
    void setup();

    // Clear the screen and render the road, vehicles afresh
    void render();
    void renderRoad();
    void renderVehicle(Vehicle* vehicle);
    
    // Returns the time since start
    float getTime();
};

#endif
