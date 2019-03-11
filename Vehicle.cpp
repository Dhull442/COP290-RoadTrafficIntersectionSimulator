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

void Vehicle::updatePos(double delT,bool limit){
  double unrestrictedNewPosition = this->currentPosition.first + (this->currentSpeed)*(delT) + (0.5)*(this->acceleration)*(delT)*(delT);
  if(limit){
    double obstacle = this->parentRoad->firstObstacle(this->currentPosition.first,this->currentPosition.second,this->currentPosition.second-this->width);
  if(obstacle>unrestrictedNewPosition){
    this->currentPosition.first = unrestrictedNewPosition;
  }
  else
    this->currentPosition.first = obstacle;}
  else{
    this->unrestrictedposition.first = unrestrictedNewPosition;
  }
}
// bool operator< (Vehicle v){
//   return Vehicle::currentPosition.first < v.currentPosition.first;
// }
// int main(){
//   Vehicle mine("Car",2,2);
//   std::cout << mine.skill << std::endl;
// }
