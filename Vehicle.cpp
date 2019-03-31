#include <bits/stdc++.h>
#include "Vehicle.h"
#include "Road.h"
Vehicle::Vehicle(){
  // Initializing defaults to -1, reinitialize on adding to a road
  this->length = -1;
  this->width = -1;
  this->maxspeed = -1;
  this->acceleration = -1;
  this->skill = -1;
  this->safedistance = -1;
  this->parentRoad = NULL;
  this->color_rgb.push_back(0);
  this->color_rgb.push_back(0);
  this->color_rgb.push_back(0);
  // Intialize position
  this->currentPosition = std::make_pair(0,0);
  this->unrestrictedposition = this->currentPosition;
  this->processed=false;
  this->delT = 0;
  this->currentLane = std::make_pair(0,0);
  this->velLimit = this->maxspeed;
  this->useLimit = false;
}

Vehicle::Vehicle(std::string type, double length, double width): Vehicle(){
  this->type = type;
  this->length = length;
  this->width = width;
}

Vehicle::Vehicle(std::string type):Vehicle(){
  this->type = type;
}

// Constructs a copy of the Vehicle
void Vehicle::reConstruct(){
  if (parentRoad == NULL) {
    std::cout << "[ ERROR ] Road is not specified for this vehicle" <<std::endl;
    std::exit(1);
  } else {
    if(this->length == -1){
        this->length = this->parentRoad->default_length;
    }
    if(this->width  == -1){
      this->width = this->parentRoad->default_width;
    }
    if(this->maxspeed == -1 || this->maxspeed > this->parentRoad->default_maxspeed){
      this->maxspeed = this->parentRoad->default_maxspeed;
    }
    if(this->acceleration == -1 || this->acceleration > this->parentRoad->default_acceleration){
      this->acceleration = this->parentRoad->default_acceleration;
    }
    if(this->skill == -1){
      this->skill = this->parentRoad->default_skill;
    }
    if(this->safedistance == -1){
      this->safedistance = this->parentRoad->default_safety_distance;
    }
  }
}

// Sets the color of the vehicle
void Vehicle::setColor(std::string color){
  // COLORS: GREEN,RED,BLUE,ORANGE,PINK,YELLOW,PURPLE,WHITE
  if(!color.compare("GREEN")){
    this->color = color;
    this->color_rgb[0] = 11;
    this->color_rgb[1] = 229;
    this->color_rgb[2] = 8;
    return;
  }
  if(!color.compare("RED")){
    this->color = color;
    this->color_rgb[0] = 237;
    this->color_rgb[1] = 32;
    this->color_rgb[2] = 32;
    return;
  }
  if(!color.compare("BLUE")){
    this->color = color;
    this->color_rgb[0] = 7;
    this->color_rgb[1] = 105;
    this->color_rgb[2] = 231;
    return;
  }
  if(!color.compare("ORANGE")){
    this->color = color;
    this->color_rgb[0] = 242;
    this->color_rgb[1] = 129;
    this->color_rgb[2] = 16;
    return;
  }
  if(!color.compare("PINK")){
    this->color = color;
    this->color_rgb[0] = 254;
    this->color_rgb[1] = 110;
    this->color_rgb[2] = 206;
    return;
  }
  if(!color.compare("YELLOW")){
    this->color = color;
    this->color_rgb[0] = 252;
    this->color_rgb[1] = 235;
    this->color_rgb[2] = 83;
    return;
  }
  if(!color.compare("PURPLE")){
    this->color = color;
    this->color_rgb[0] = 112;
    this->color_rgb[1] = 4;
    this->color_rgb[2] = 253;
    return;
  }
  if(!color.compare("WHITE")){
    this->color = color;
    this->color_rgb[0] = 255;
    this->color_rgb[1] = 255;
    this->color_rgb[2] = 255;
    return;
  }
  {
    std::cout<<"[ ERROR ] color can only be GREEN/RED";
  }
}

// Update the parameters of the Vehicles based on the time increment -- TO BE EDITED
void Vehicle::updatePos(double delT) {
    this->delT = delT;
    // Firstly, update the velocities and positionx
    // Check if the velocity limit is, in fact, exceeded
    if (this->useLimit) {
        // If we are using the limit, get the part time until it accelerates
        double partTime = (this->velLimit - this->currentSpeed)/(this->a);
        this->currentPosition.first += (this->currentSpeed)*partTime + 0.5*(this->a)*(partTime)*(partTime) + (this->velLimit)*(delT - partTime);
    } else {
        this->currentPosition.first += (this->currentSpeed)*delT + 0.5*(this->a)*(delT)*(delT);
    }

    if (this->useLimit) {
        // If the limit is being breached, this is the new max speed
        this->currentSpeed = this->velLimit;
    } else {
        // If no breach, the usual laws hold
        this->currentSpeed += (this->a)*delT;
    }

    // Get the distance between this and next nearest obstacle
    this->closestDistance = this->parentRoad->firstObstacle(this, delT) - this->safedistance;

    // Now we need to find the value of acceleration
    double A = delT*delT/(2*this->acceleration);
    double B = (delT*delT/2) + (this->currentSpeed*delT/this->acceleration);
    double C = this->currentSpeed*this->currentSpeed/(2*this->acceleration) + this->currentSpeed*delT - this->closestDistance;
    // Sqrt Discriminant of above QE
    double Disc = sqrt(B*B - 4*A*C);

    // Define the limit on the velocity
    this->velLimit = this->closestDistance/(2*delT);
    this->useLimit = false;
    // Get the accleration value from the equation
    this->a = (-B + Disc)/(2*A);

    // Check if the accleration exceeds a_max
    if (this->a > this->acceleration) {
        this->a = this->acceleration;
    }

    if (this->closestDistance < this->safedistance/20) {
        this->a = 0;
        this->currentSpeed = 0;
    }

    // Look at the future speed and its bounds
    double futureSpeed = this->currentSpeed + this->a*delT;

    if (futureSpeed > this->velLimit) {
        this->useLimit = true;
    }

    if (futureSpeed > this->maxspeed && this->maxspeed < this->velLimit) {
        this->useLimit = true;
        this->velLimit = this->maxspeed;
    }

    this->processed = true;
    if(this->currentPosition.first - this->length > this->parentRoad->length){
      this->isOnRoad = false;
    };
}

// DK what this does -- WILL BE EDITED
double Vehicle::activation_function(double speed){ // equivalent to newton's 3rd law
  return speed + 0.5;
}
