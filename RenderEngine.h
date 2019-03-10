#ifdef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include <bits/stdc++.h>
#include "Vehicle.h"
// #include "Road.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include "linmath.h"

// This class takes a road object and renders it
class RenderEngine {
  public:
    // Road* targetRoad;
    bool isInitialized;
    // The variable which store the OpenGL window
    GLFWwindow* window;

    // RenderEngine(Road* targetRoad);
    RenderEngine();

    // The error callback function
    void error_callback(int error, const char* description);

    // Initialize the variables
    void setup();

    // Clear the screen and render the road afresh
    void render();
}

#endif
