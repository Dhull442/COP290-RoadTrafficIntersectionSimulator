#include <bits/stdc++.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include "Vehicle.h"

Road::Road(){
  this->id = 0;
  this->length = 0.0;
  this->width = 0.0;
  // Signal is red by default
  this->signal = "RED";
  this->signalPosition = 0.0;

  // OpenGL part
  // Default window lengths and widths
  this->window_length = 640;
  this->window_height = 480;
}

Road::Road(int id, float length, float width):Road(){
    this->id = id;
    this->length = length;
    this->width = width;
}
Road::Road(int id):Road(){
    this->id = id;
}

void Road::setDefaults(float maxspeed, float acceleration,float length, float width,int skill,float speed_limit){
    // Input correclty < No checks here >
    this->default_maxspeed = maxspeed;
    this->default_acceleration = acceleration;
    this->default_length = length;
    this->default_width = width;
    this->default_skill = skill;
    this->speed_limit = speed_limit;
}

// For adding vehicle
void Road::addVehicle(Vehicle* vehicle) {
    vehicles.push_back(vehicle);
    // Add the road to the vehicle
    vehicle->onRoad = true;
    vehicle->parentRoad = this;
    // Update the values
    vehicle->currentPosition = this->queuePos;
    this->queuePos -= this->bufferLength;
}

// Runs the simulation and renders the road
void Road::runSim(float t) {
    // Run until time is exhausted
    float beginTime = glfwGetTime();
    float oldTime = glfwGetTime();
    float currentTime = glfwGetTime();

    while(currentTime - beginTime < t) {
        if(glfwWindowShouldClose(this->window)) {
            // Check if a window close signal is received
            glfwDestroyWindow(this->window);
            glfwTerminate();
        }

        this->isClear = true;
        this->updateUnrestrictedpositions(currentTime - oldTime);
        // Update positions of each car
        for(auto v: this->vehicles) {
            v->updatePos(currentTime - oldTime,true);
            this->isClear = this->isClear && (((v->length + v->currentPos) > this->length) || (v->currentPos < 0));
        }
        oldTime = currentTime;
        currentTime = glfwGetTime();
        // Render the current state
        renderRoad();
    }
}

// Gives first obstacle position in the given window
float Road::firstObstacle(float startPos, float topRow, float botRow ){
  float position=this->length;
  for(auto v : this->vehicles ){
    if(v->unrestrictedposition.second < topRow || (v->unrestrictedposition.second-v->width)>botRow){
      float back = (v->unrestrictedposition.first-v->length);
      if(position > back && back > startPos ){
        position = back;
      }
    }
  }
  return position;
}

// Updates the unrestricted new positions of every vehicle
void Road::updateUnrestrictedpositions(float delT){
  for(auto v : this -> vehicles ){
    v->updatePos(delT,false);
  };
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
    this->window = glfwCreateWindow(this->window_length, this->window_height, "SimView", NULL, NULL);
    if (!this->window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Make this context current
    glfwMakeContextCurrent(this->window);

    // Set the key_callback function
    glfwSetKeyCallback(window, Road::key_callback);
}

void Road::renderRoad() {
    // Get frameBuffer attributes
    float ratio;
    int frame_height, frame_width;
    glfwGetFramebufferSize(this->window, &window_height, &window_width);
    ratio = frame_width/(float)frame_height;

    // Create a blank viewport
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render the road as a rectangle


    // Swap the buffers, to display rendered stuff on the screen
    glfwSwapBuffers(this->window);
    glfwPollEvents();
}
