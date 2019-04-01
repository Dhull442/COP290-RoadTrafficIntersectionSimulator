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
  this->emergency = false;
  this->ascii_color="\033[1;30m";
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
    this->ascii_color = "\033[1;32m";
    return;
  }
  if(!color.compare("RED")){
    this->color = color;
    this->color_rgb[0] = 237;
    this->color_rgb[1] = 32;
    this->color_rgb[2] = 32;
    this->ascii_color = "\033[1;31m";
    return;
  }
  if(!color.compare("BLUE")){
    this->color = color;
    this->color_rgb[0] = 7;
    this->color_rgb[1] = 105;
    this->color_rgb[2] = 231;
    this->ascii_color = "\033[1;34m";
    return;
  }
  if(!color.compare("ORANGE")){
    this->color = color;
    this->color_rgb[0] = 242;
    this->color_rgb[1] = 129;
    this->color_rgb[2] = 16;
    this->ascii_color = "\033[1;31m";
    return;
  }
  if(!color.compare("PINK")){
    this->color = color;
    this->color_rgb[0] = 254;
    this->color_rgb[1] = 110;
    this->color_rgb[2] = 206;
    this->ascii_color = "\033[1;35m";
    return;
  }
  if(!color.compare("YELLOW")){
    this->color = color;
    this->color_rgb[0] = 252;
    this->color_rgb[1] = 235;
    this->color_rgb[2] = 83;
    this->ascii_color = "\033[1;33m";
    return;
  }
  if(!color.compare("PURPLE")){
    this->color = color;
    this->color_rgb[0] = 112;
    this->color_rgb[1] = 4;
    this->color_rgb[2] = 253;
    this->ascii_color = "\033[1;36m";
    return;
  }
  if(!color.compare("WHITE")){
    this->color = color;
    this->color_rgb[0] = 255;
    this->color_rgb[1] = 255;
    this->color_rgb[2] = 255;
    this->ascii_color = "\033[1;37m";
    return;
  }
  {
    std::cout<<"[ ERROR ] color can only be GREEN/RED";
  }
}

// Update the parameters of the Vehicles based on the time increment -- TO BE EDITED
void Vehicle::updatePos(double delT, double globalTime) {
    this->processed = true;
    this->delT = delT;
    // Firstly, update the velocities and positionx
    // Check if the velocity limit is, in fact, exceeded
    if (this->useLimit) {
        // std::cout << "LIMIT BREACHED " << this->velLimit << std::endl;
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
    this->closestDistance = this->parentRoad->firstObstacle(this, delT, globalTime) - this->safedistance;

    // Now we need to find the value of acceleration
    double A = delT*delT/(2*this->acceleration);
    double B = (delT*delT/2) + (this->currentSpeed*delT/this->acceleration);
    double C = this->currentSpeed*this->currentSpeed/(2*this->acceleration) + this->currentSpeed*delT - this->closestDistance;
    // Sqrt Discriminant of above QE
    double Disc1 = B*B - 4*A*C;
    // std::cout << "Defined A, B, C, D" << std::endl;

    // Define the limit on the velocity
    this->velLimit = this->closestDistance/(2*delT);
    this->useLimit = false;

    if (this->closestDistance < this->safedistance/20 || Disc1 < 0) {
      this->a = 0;
      this->currentSpeed = 0;
      this->emergency = true;
      return;
    } else {
      this->emergency = false;
    }

    double Disc = sqrt(Disc1);
    // Get the accleration value from the equation
    this->a = (-B + Disc)/(2*A);

    // Check if the accleration exceeds a_max
    if (this->a > this->acceleration) {
        this->a = this->acceleration;
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


    // Check if we are changing the Lane, update stuff
    if (this->changingLane) {
      // The total distance to be travelled
      double delY = this->parentRoad->width/(float)this->parentRoad->lanes*1.159  ;

      // Update the positions
      this->verticalPosition += delT*this->verticalSpeed;
      this->currentPosition.second += this->changeDirection*delT*this->verticalSpeed;

      // Check if lane changing is complete
      if (abs(delY-this->verticalPosition) < 0.001*delY) {
        // Lane changing is complete
        std::cout << "Lange changing is complete " << this->color << " " << this->type << std::endl;
        this->changingLane = false;
        this->verticalPosition = 0;
        this->lastLaneChange = globalTime;

        if (this->changeDirection == -1) {
          // The shift was toward the bottom
          // Update the lanes
          this->parentRoad->removeFromLane(this, this->currentLane.first);
          this->currentLane.first++;
        } else {
          // Shift was toward the top
          this->parentRoad->removeFromLane(this, this->currentLane.second);
          this->currentLane.second--;

        }
      } else {
        // Update the parameters -- otherwise
        this->verticalSpeed = this->speedRatio*this->currentSpeed;
        double limSpeed = (delY-this->verticalPosition)/delT;
        if (this->verticalSpeed > limSpeed) {
          this->verticalSpeed = limSpeed;
        }
      }
    }

    this->parentRoad->printLanes();
}

void Vehicle::changeLane(double delT, double globalTime) {
  // std::cout << "CHANGING LANES" << std::endl;
  if (globalTime - this->lastLaneChange >= this->timeGap && !this->changingLane && this->currentPosition.first >= 0) {
      // Check if a lane change is isPossible -- downwards
      this->front = NULL;
      this->back = NULL;
      bool hasSpace = this->parentRoad->getAdjVehicles(this, 1, delT, globalTime);
      if (this->front != NULL) {std::cout << front->color << " " << front->type << " ";} else {std::cout << "NULL ";}
      if (this->back != NULL) {std::cout << back->color << " " << back->type << " ";} else {std::cout << "NULL ";}
      std::cout << std::endl;
      if (hasSpace && Vehicle::isPossible(delT)) {
        this->changingLane = true;
        this->verticalPosition = 0;
        this->verticalSpeed = 0;
        this->currentLane.second++;
        this->changeDirection = -1;
        // Update the lanesng
        this->parentRoad->insertInLane(front, this->currentLane.second, this);
        // Lane change code goes here
        return;
      }

      // Check if it is possible to change in the other direction

      hasSpace = this->parentRoad->getAdjVehicles(this, -1, delT, globalTime);
      if (this->front != NULL) {std::cout << front->color << " " << front->type << " ";} else {std::cout << "NULL ";}
      if (this->back != NULL) {std::cout << back->color << " " << back->type << " ";} else {std::cout << "NULL ";}
      std::cout << std::endl;
      if (hasSpace && Vehicle::isPossible(delT)) {
        this->changingLane = true;
        this->verticalPosition = 0;
        this->verticalSpeed = 0;
        this->currentLane.first--;
        this->changeDirection = 1;
        this->parentRoad->insertInLane(front, this->currentLane.first, this);
        // Lane change code goes here
        return;
      }
    }
}

bool Vehicle::isPossible(double delT) {
  std::cout << "Checking possibility for " << this->color << " " << this->type << std::endl;
  if (this->front == NULL) {
    std::cout << "There is nothing in the front " << std::endl;
    if (this->parentRoad->isRed()) {
      std::cout << "Signal found in the front" << std::endl;
      // There is a signal in the front
      double d1 = this->parentRoad->signalPosition - this->currentPosition.first;
      double d1p = d1 - this->safedistance - (this->currentSpeed)*delT - 0.5*(delT)*(delT)*(this->a);
      if (d1p >= 0.01*this->safedistance) {
        std::cout << "Front OK" << std::endl;
        if (this->back == NULL) {
          std::cout << "There is nothing in the back" << std::endl;
          return true;
        } else {
          std::cout << "There is a vehicle at the back" << std::endl;
          double d2 = this->currentPosition.first - this->length-back->currentPosition.first;
          double d2p = d1p - this->back->safedistance - (this->back->currentSpeed*delT + 0.5*delT*delT*this->back->a) + (this->currentSpeed*delT + 0.5*delT*delT*this->a);
          if (d2p >= 0.01*this->back->safedistance) {
            std::cout << "Back vehicle is OK" << std::endl;
            return true;
          } else {
            std::cout << "Back vehicle is not OK" << std::endl;
            return false;
          }
        }
      } else {
        std::cout << "Signal failed " << std::endl;
        return false;
      }
    } else {
      std::cout << "There is nothing in the front" << std::endl;
      if (this->back == NULL) {
        std::cout << "There is nothing in the back" << std::endl;
        return true;
      }


      // There is no signal'
      double d2 = this->currentPosition.first - this->length-this->back->currentPosition.first;
      double d2p = d2 - this->back->safedistance - (this->back->currentSpeed*delT + 0.5*delT*delT*this->back->a) + (this->currentSpeed*delT + 0.5*delT*delT*this->a);
      if (d2p >= 0.01*this->back->safedistance) {
        std::cout << "Back vehicle is OK" << std::endl;
        return true;
      } else {
        std::cout << "Back vehicle is not OK" << std::endl;
        return false;
      }
    }
  } else {
    std::cout << "There is a car in the front " << std::endl;
    double d1 = this->front->currentPosition.first - this->front->length - this->currentPosition.first;
    double d1p = d1 - this->safedistance - (this->currentSpeed)*delT - 0.5*(delT)*(delT)*(this->a) + (this->front->currentSpeed*delT + 0.5*delT*delT*this->front->a);
    if (d1p < 0.01*this->safedistance) {
      std::cout << "Failed for the front " << std::endl;
      return false;
    }

    if (back == NULL) {
      std::cout << "There is nothing in the back" << std::endl;
      return true;
    }

    double d2 = this->currentPosition.first - this->length - back->currentPosition.first;
    double d2p = d1p - this->back->safedistance - (this->back->currentSpeed*delT + 0.5*delT*delT*this->back->a*this->back->a) + (this->currentSpeed*delT + 0.5*delT*delT*this->a);
    if (d2p >= 0.01*back->safedistance) {
      std::cout << "Back OK" << std::endl;
      return true;
    } else {
      std::cout << "Back fails" << std::endl;
      return false;
    }
  }
}

// DK what this does -- WILL BE EDITED
double Vehicle::activation_function(double speed){ // equivalent to newton's 3rd law
  return speed + 0.5;
}
