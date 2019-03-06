#include <bits/stdc++.h>
#include "Vehicle.h"
#include "Roah.h"
Vehicle::Vehicle(){
  // Initializing defaults
  this->type = this->default_type;
  this->length = this->default_length;
  this->width = this->default_width;
  this->maxspeed = this->default_maxspeed;
  this->acceleration = this->default_acceleration;
  this->skill = this->default_skill;
  this->parentRoad = NULL;

  // Intialize position
  this->currentPosition = make_pair(0,0);
  this->unrestrictedposition = this->currentPosition;
}
Vehicle::Vehicle(std::string type, float length, float width): Vehicle(){
  this->type = type;
  this->length = length;
  this->width = width;
}
void Vehicle::updatePos(float delT,bool limit){
  if(limit){
    float obstacle = this.parentRoad->firstObstacle(this->currentPosition.first,this->currentPosition.second,this->currentPosition.second-this->width);
  };
  float unrestrictedNewPosition = this->currentPosition.first + (this->currentSpeed)*(delT) + (0.5)*(this->acceleration)*(delT)*(delT);
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
