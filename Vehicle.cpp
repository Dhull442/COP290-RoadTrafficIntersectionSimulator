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
  this->color_rgb.push_back(0);
  this->color_rgb.push_back(0);
  this->color_rgb.push_back(0);
  // Intialize position
  this->currentPosition = std::make_pair(0,0);
  this->unrestrictedposition = this->currentPosition;
  this->processed=false;
  this->delT = 0;
  this->currentLane = std::make_pair(0,0);
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
    if(this->maxspeed == -1 || this->maxspeed > this->parentRoad->default_maxspeed){
      this->maxspeed = this->parentRoad->default_maxspeed;
    }
    if(this->acceleration == -1 || this->acceleration > this->parentRoad->default_acceleration){
      this->acceleration = this->parentRoad->default_acceleration;
    }
    if(this->skill == -1){
      this->skill = this->parentRoad->default_skill;
    }
  }
}

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

void Vehicle::updatePos(bool limit){
  double delT = this->delT;
  double unrestrictedNewPosition = this->currentPosition.first + (this->currentSpeed)*(delT) + (0.5)*(this->acceleration)*(delT)*(delT);
  if(this->currentSpeed >= this->maxspeed){
    unrestrictedNewPosition = this->currentPosition.first + (this->currentSpeed)*delT;
  }

  if(limit){
    bool free = true; // false when vehicle stopped due to another vehicle
    double delX= this->currentPosition.first;
    double obstacle = this->parentRoad->firstObstacle(this);
    if(this->parentRoad->isRed()){
      if(obstacle>this->parentRoad->signalPosition){
        obstacle = this->parentRoad->signalPosition-0.5;
        std::cout << "Road sign"<<std::endl;
      }
    }
    {
      {
        std::cout << this->type << " has obs at "<<this->currentSpeed<<" and free at "<<this->acceleration<<" s = "<<this->currentPosition.first<<std::endl;
      if(obstacle-this->currentPosition.first>2){
        unrestrictedNewPosition = (this->currentSpeed)*(delT) + (0.5)*(this->acceleration)*(delT)*(delT);
        if(this->currentSpeed >= this->maxspeed){
          unrestrictedNewPosition = (this->currentSpeed)*delT;
        }
        this->currentPosition.first += unrestrictedNewPosition;
        }
      else{
        std::cout << "Decelerate ";
        double d = obstacle - this->currentPosition.first;
        std::cout << d <<" ";
        double a = std::pow(this -> currentSpeed,2)/(2*(d));
        double change = this->currentSpeed*delT - (0.5)*a*delT*delT;
        if(change > 0)
          this->currentPosition.first += change;
        std::cout <<this->currentPosition.first<<" "<<std::endl;
        this->stoped = true;
        free = false;
      }
    }
    delX -= this->currentPosition.first;
    if(delT > 0){
      this->currentSpeed = (-1*delX)/delT;
    }
    std::cout << "after change "<<this->currentPosition.first<<" "<<this->currentSpeed<<" "<<this->acceleration << std::endl;
    if(!free && !(this->parentRoad->isRed())){
      if(this->currentSpeed > 0){
      this->parentRoad->changeLane(this);

    }}}
  if((this->currentPosition.first - this->length) >= this->parentRoad->length){
    this->isOnRoad = false;
  }
  // this->stopdist = this->activation_function(this->currentSpeed);
  }
  else{
    this->unrestrictedposition.first = unrestrictedNewPosition;
  }
  this->processed = limit;
}

double Vehicle::activation_function(double speed){ // equivalent to newton's 3rd law
  return speed + 0.5;
}
// bool operator< (Vehicle v){
//   return Vehicle::currentPosition.first < v.currentPosition.first;
// }
// int main(){
//   Vehicle mine("Car",2,2);
//   std::cout << mine.skill << std::endl;
// }
