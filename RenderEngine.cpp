#include <bits/stdc++.h>
#include "Vehicle.h"
#include "Road.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include "linmath.h"

RenderEngine::RenderEngine(Road* targetRoad) {
    this->targetRoad = targetRoad;
}

RenderEngine::RenderEngine() {
    // Empty function for debugging purposes
}

void RenderEngine::error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

// Function which receives the key events
static void RenderEngine::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        // Set the close_flag to true
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

// Initalize GLFW stuff
void RenderEngine::setup() {
    // Initialize GLFW, return error otherwise
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    // Set the error_callback function
    glfwSetErrorCallback(error_callback);

    // Create a window, context
    this->window = glfwCreateWindow(640, 480, "TrafficSim", NULL, NULL);
    if (!this->window) {
        // Context creation failed
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    this->isInitialized = true;
    // Make the context current
    glfwMakeContextCurrent(window);
    // Set the swap interval
    glfwSwapInterval(1);
    // Set the key_callback method
    glfwSetKeyCallback(this->window, key_callback);
}

void RenderEngine::render() {
    // While the window is not closed do this
    while(!glfwWindowShouldClose(this->window)) {
        // Setup the frame
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin();
        glRectd(0, 0, 1, 1);
        glEnd();

        // Swap buffers and check for events
        glfwSwapBuffers(this->window);
        glfwPollEvents();
    }
    glfwTerminate();
    std::cout << "Simulation Done..." << std::endl;
}
