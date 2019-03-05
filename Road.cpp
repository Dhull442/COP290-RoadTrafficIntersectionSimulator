#include <bits/stdc++.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include "Vehicle.h"

Road::Road(int id, float length, float width) {
    self.id = id;
    self.length = length;
    self.width = width;
    // Signal is red by default
    self.signal = "RED";
    // Default window lengths and widths
    self.window_length = 640;
    self.window_height = 480;
}

void Road::addVehicle(Vehicle* vehicle) {
    vehicles.push_back(vehicle);
    // Add the road to the vehicle
    vehicle->onRoad = true;
    vehicle->parentRoad = this;
    // Update the values
    vehicle->currentPos = self.queuePos;
    self.queuePos -= self.bufferLength;
}

// Runs the simulation and renders the road
void Road::runSim(float t) {
    // Run until time is exhausted
    float beginTime = glfwGetTime();
    float oldTime = glfwGetTime();
    float currentTime = glfwGetTime();

    while(currentTime - beginTime < t) {
        if(glfwWindowShouldClose(self.window)) {
            // Check if a window close signal is received
            glfwDestroyWindow(self.window);
            glfwTerminate();
        }

        self.isClear = true;
        // Update positions of each car
        for(auto v: self.vehicles) {
            v->updatePos(currentTime - oldTime);
            self.isClear = self.isClear && (((v->length + v->currentPos) > self.length) || (v->currentPos < 0));
            oldTime = currentTime;
            currentTime = glfwGetTime();
        }

        // Render the current state
        renderRoad();
    }
}

// The error_callback function prints out the error and exits with non-zero status
static void Road::error_callback(int error, const char* description) {
    std::cerr << description << std::endl;
    std::exit(1);
}

static void Road::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Exit when the escape key is pressed
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwWindowShouldClose(window, GL_TRUE);
    }
}

void Road::setupRoad() {
    // Initialize GLFW
    glfwInit();
    glEnable(GL_DEPTH_TEST);
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    // Set the error_callback function
    glfwSetErrorCallback(Road::error_callback);

    // Create a new window
    self.window = glfwCreateWindow(self.window_length, self.window_height, "SimView", NULL, NULL);
    if (!self.window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Make this context current
    glfwMakeContextCurrent(self.window);

    // Set the key_callback function
    glfwSetKeyCallback(window, Road::key_callback);
}

void Road::renderRoad() {
    // Get frameBuffer attributes
    float ratio;
    int frame_height, frame_width;
    glfwGetFramebufferSize(self.window, &window_height, &window_width);
    ratio = frame_width/(float)frame_height;

    // Create a blank viewport
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render the road as a rectangle


    // Swap the buffers, to display rendered stuff on the screen
    glfwSwapBuffers(self.window);
    glfwPollEvents();
}
