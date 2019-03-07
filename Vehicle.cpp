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
  this->parentRoad = NULL;

  // Intialize position
  this->currentPosition = std::make_pair(0,0);
  this->unrestrictedposition = this->currentPosition;
}

Vehicle::Vehicle(std::string type, double length, double width): Vehicle(){
  this->type = type;
  this->length = length;
  this->width = width;
}

Vehicle::Vehicle(std::string type):Vehicle(){
  this->type = type;
}

void Vehicle::reConstruct(){
  if(parentRoad == NULL){
    std::cout << "[ ERROR ] Road is not specified for this vehicle" <<std::endl;
    std::exit(1);
  }
  else{
    if(this->length == -1){
    this->length = this->parentRoad->default_length;
    }
    if(this->width  == -1){
      this->width = this->parentRoad->default_width;
    }
    if(this->maxspeed == -1){
      this->maxspeed = this->parentRoad->default_maxspeed;
    }
    if(this->acceleration == -1){
      this->acceleration = this->parentRoad->default_acceleration;
    }
    if(this->skill == -1){
      this->skill = this->parentRoad->default_skill;
    }
  }
}

void Vehicle::updatePos(double delT,bool limit){
  if(limit){
    double obstacle = this.parentRoad->firstObstacle(this->currentPosition.first,this->currentPosition.second,this->currentPosition.second-this->width);
  };
  double unrestrictedNewPosition = this->currentPosition.first + (this->currentSpeed)*(delT) + (0.5)*(this->acceleration)*(delT)*(delT);
  if(limit){
  if(obstacle>unrestrictedNewPosition){
    this->currentPosition.first = unrestrictedNewPosition;
  }
  else
    this->currentPosition.first = obstacle;}
  else{
    this->unrestrictedposition.first = unrestrictedposition;
  }
}
int main(){
  Vehicle mine("Car",2,2);
  std::cout << mine.skill << std::endl;
}
