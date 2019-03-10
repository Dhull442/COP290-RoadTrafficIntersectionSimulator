#include <bits/stdc++.h>
#include "Vehicle.h"
#include "Road.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include "RenderEngine.h"

// #define GLFW_TRUE true
RenderEngine::RenderEngine(Road* targetRoad) {
    std::cout << "Instantiated RenderEngine for road " << targetRoad->id << std::endl;
    this->targetRoad = targetRoad;
    this->scaling = 20;
    // Set the default background color
    this->bgcolor.push_back(1.0f);
    this->bgcolor.push_back(0.968f);
    this->bgcolor.push_back(0.3529f);
    // Set the monitor Parameters
    this->monitorWidth = 1280;
    this->monitorHeight = 800;
}

// Default constructor
RenderEngine::RenderEngine() {
    // Do nothing
    std::cout << "RenderEngine() default constructor called" << std::endl;
}

void RenderEngine::error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

// Function which receives the key events
void RenderEngine::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        // Set the close_flag to true
        std::cout << "Exit keypress" << std::endl;
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

// Initalize GLFW stuff
void RenderEngine::setup() {
    std::cout << "Setting up the RenderEngine" << std::endl;
    // Initialize GLFW, return error otherwise
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    // Set the error_callback function
    glfwSetErrorCallback(RenderEngine::error_callback);

    // Create a window, context
    std::cout << "Creating a window" << std::endl;
    RenderEngine::window = glfwCreateWindow(this->monitorWidth, this->monitorHeight, "TrafficSim", glfwGetPrimaryMonitor(), NULL);

    if (!RenderEngine::window) {
        // Context creation failed
        glfwTerminate();
        std::cout << "Could not create window" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    
    RenderEngine::isInitialized = true;
    // Make the context current
    glfwMakeContextCurrent(RenderEngine::window);
    // Set the swap interval
    glfwSwapInterval(1);
    // Set the key_callback method
    glfwSetKeyCallback(RenderEngine::window, RenderEngine::key_callback);
}

float RenderEngine::getTime() {
    float time = glfwGetTime();
    std::cout << "The current time is " << time << std::endl;
    return time;
}

void RenderEngine::render() {
    std::cout << "Calling the render function" << std::endl;
    // std::cout << glfwWindowShouldClose(RenderEngine::window) << std::endl;
    std::cout << "Checking condition" << std::endl;
    // While the window is not closed do this
    // if(!glfwWindowShouldClose(RenderEngine::window)) {
        // Setup the frame
        std::cout << "Setting up the frame here" << std::endl;  
        float ratio;
        int width, height;
        std::cout << "Getting framebuffer size" << std::endl;
        glfwGetFramebufferSize(RenderEngine::window, &width, &height);
        std::cout << "Framebuffer size obtained" << std::endl;

        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        // Render the background
        std::cout << "Rendering the background" << std::endl;
        glClearColor((float)this->bgcolor[0]/255.0f, (float)this->bgcolor[1]/255.0f, (float)this->bgcolor[2]/255.0f, 1.0f);

        // Render the road
        RenderEngine::renderRoad();

        // Iterate over the vehicles
        for(auto v: this->targetRoad->vehicles) {
            renderVehicle(v);
        }

        // Swap buffers and check for events
        glfwSwapBuffers(RenderEngine::window);
        glfwPollEvents();
        std::cout << "Rendering Done..." << std::endl;
   // } else {
    //    std::cout << "Window is closed" << std::endl;
     //   glfwTerminate();
     //   std::exit(1);
    // }
}

void RenderEngine::renderRoad() {
    std::cout << "Rendering the road" << std::endl;
    // Render the road in black
    glColor3f(0.0f, 0.0f, 0.0f);
    float ycoord = this->targetRoad->width*this->scaling/(float)this->monitorHeight;
    float xcoord = (this->targetRoad->length*this->scaling - (int)(this->monitorWidth/2))/(float)((int)(this->monitorWidth/2));
    glRectd(-1.0f, ycoord, xcoord, -ycoord);

    // Render the signal in the remaining part
    glColor3f((float)this->targetRoad->signal_rgb[0]/255.0f,
              (float)this->targetRoad->signal_rgb[0]/255.0f,
              (float)this->targetRoad->signal_rgb[0]/255.0f);
    glRectd(xcoord, ycoord, 1.0f, -ycoord);
}

void RenderEngine::renderVehicle(Vehicle* vehicle) {
    std::cout << "Rendering the Vehicle" << std::endl;
    if (vehicle->isOnRoad) {
        std::cout << "The vehicle is on the road" << std::endl;
        // Render only if the vehicle is on the Road
        float x = -1.0 + (float)vehicle->currentPosition.first*this->scaling*2/(float)(this->monitorWidth);
        float y = (float)this->scaling*(this->targetRoad->width-2*vehicle->currentPosition.second)/(float)this->monitorHeight;
        float delx = 2*vehicle->width*this->scaling/(float)this->monitorWidth;
        float dely = 2*vehicle->length*this->scaling/(float)this->monitorHeight;
        // Set the correct color
        glColor3f(vehicle->color_rgb[0], vehicle->color_rgb[1], vehicle->color_rgb[2]);
        // Render the rectangle
        glRectd(x, y, x - delx, y - dely);
    }
}

// int main() {
//     RenderEngine engine();
//     engine.setup();
//     engine.render();
// }
