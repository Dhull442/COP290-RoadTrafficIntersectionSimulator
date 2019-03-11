#include <bits/stdc++.h>
#include "Vehicle.h"
#include "Road.h"
#include "RenderEngine.h"

Road::Road() {
    #ifdef RENDER_ENGINE_H
    //new(&(this->engine)) RenderEngine(this);
    RenderEngine newengine(this);
    this->engine = newengine;
    this->engine.setup();
    #endif
    this->lanes = 1;
    this->id = 0;
    this->length = 0.0;
    this->width = 0.0;
    // Signal is red by default
    this->signal = "RED";
    this->signal_rgb.push_back(0);
    this->signal_rgb.push_back(0);
    this->signal_rgb.push_back(0);
    this->setSignal(this->signal);
    this->signalPosition = 0.0;
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
void Road::addVehicle(Vehicle* vehicle,std::string color) {  // Vehicle from template

    Vehicle* newVehicle = new Vehicle(*vehicle); // Make a copy from vehicle template
    // std::cout<<"(New pointer = "<<newVehicle<<", template pointer="<<vehicle<<" )"<<std::endl;
    newVehicle->setColor(color);
    newVehicle->isOnRoad = true;
    newVehicle->parentRoad = this;
    newVehicle->currentPosition = this->initPosition(newVehicle);
    newVehicle->reConstruct();

    // push with insertion sort
    if(this->vehicles.size() < 1){
      this->vehicles.push_back(newVehicle);
    }
    else{
      for(int i = 0; i< this->vehicles.size();i++){
        if(newVehicle->currentPosition.second > this->vehicles[i]->currentPosition.second){
          this->vehicles.insert(this->vehicles.begin()+i,newVehicle);
          break;
        }
        if(i == this->vehicles.size() - 1){
          this->vehicles.push_back(newVehicle);
          break;
        }
      }
    }
    // Add the road to the vehicle

    // To set defaults of road if not constructed
    std::cout <<this->vehicles.back()->type <<" of "<<color<<" added"<<std::endl;

}

void Road::setSignal(std::string signal){
    if(!signal.compare("GREEN")){
        this->signal = signal;
        this->signal_rgb[0] = 11;
        this->signal_rgb[1] = 229;
        this->signal_rgb[2] = 8;
        return;
    }
    if(!signal.compare("RED")){
        this->signal = signal;
        this->signal_rgb[0] = 237;
        this->signal_rgb[1] = 32;
        this->signal_rgb[2] = 32;
        return;
    }
    {
        std::cout<<"[ ERROR ] Signal can only be GREEN/RED";
    }
}

void Road::updateSim(double delT){


  // Update unrestricted positions
  this->updateUnrestrictedpositions(delT);

  // Update positions of each car
  for(int i=0;i<this->vehicles.size();i++) {
    if(vehicles[i]->isOnRoad){
      vehicles[i]->updatePos(delT,true);
    }
  }
}

// Runs the simulation and renders the road
void Road::runSim(double delT) {
    this->engine.render(delT);
}


std::pair<double,double> Road::initPosition(Vehicle* vehicle){
  // double posx = 0;
  // std::vector<Vehicle*> beforeLine;
  // for(auto v: this->vehicles){
  //   if((v->currentPosition.first - v->length)<0) // if back end of vehicle will be beyond start.
  //     beforeLine.push_back(v);
  // }
  //
  // if(beforeLine.size()<1)
  //   return std::make_pair(0,this->width);
  // for(int i=0;i<beforeLine.size();i++){
  //   if(i==0){
  //       if()
  //   }
  // }
  // posx=this->length;
  // for(int i=0;i<this->vehicles.size();i++){
  //   if((vehicles[i]->currentPosition.first - vehicles[i]->length) < posx ) // back End of vehicle
  //   {
  //     posx = (vehicles[i]->currentPosition.first - vehicles[i]->length);
  //
  //   }
  //   if(posx>0){
  //       return std::make_pair(0,this->width);
  //   }
  //   else
  //   return std::make_pair(posx,this->width);

  // Lane implementation
  double lanewidth = this->width / (double) this->lanes;
  double lanepos=this->width;
  int laneno=this->lanes - 1;
  double posy=this->width,posx=0;
  for( int i=0;laneno >= 0 ;i++ ){
      if(i > this->vehicles.size()){
          posx = 0;
          posy = (laneno) * lanewidth;
          break;
      }
      else{
           v = this->vehicles[i];
      if(v->currentPosition.second <= lanewidth*laneno){
          laneno--;
      }
      double back = (v->currentPosition.first-v->length);
      if(posx < back && back <= 0){
          posx = back;
          posy = (laneno+1) * lanewidth;
      }
    }
  }
  return std::make_pair(posx,posy);
}

// Gives first obstacle position in the given window
double Road::firstObstacle(double startPos,double length, double topRow, double botRow ){
    double position=this->length+2*length;
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
    if(v->isOnRoad)
      v->updatePos(delT,false);
  };
  // std::cout <<"Updated unrestricted positions for everyone" <<std::endl;

}

bool Road::isRed(){
    return (!this->signal.compare("RED"));
}
