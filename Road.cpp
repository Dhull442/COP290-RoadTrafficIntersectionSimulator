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

Road::Road(int id, double length, double width):Road(){
    this->id = id;
    this->length = length;
    this->width = width;
}

Road::Road(int id):Road(){
    this->id = id;
}

void Road::setDefaults(double maxspeed, double acceleration,double length, double width,int skill){
    // Input correclty < No checks here >
    this->default_maxspeed = maxspeed;
    this->default_acceleration = acceleration;
    this->default_length = length;
    this->default_width = width;
    this->default_skill = skill;
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

    // To set defaults of road if not constructed
    vehicles.back()->reConstruct();
}

// Runs the simulation and renders the road
void Road::runSim(double t) {
    // Run until time is exhausted
    double beginTime = glfwGetTime();
    double oldTime = glfwGetTime();
    double currentTime = glfwGetTime();

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
double Road::firstObstacle(double startPos, double topRow, double botRow ){
  double position=this->length;
  for(auto v : this->vehicles ){
    if(v->unrestrictedposition.second < topRow || (v->unrestrictedposition.second-v->width)>botRow){
      double back = (v->unrestrictedposition.first-v->length);
      if(position > back && back > startPos ){
        position = back;
      }
    }
  }
  return position;
}

// Updates the unrestricted new positions of every vehicle
void Road::updateUnrestrictedpositions(double delT){
  for(auto v : this -> vehicles) {
    v->updatePos(delT,false);
  }
}
