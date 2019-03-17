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
    // std::cout << "Position intialized to ("<<newVehicle->currentPosition.first<<","<<newVehicle->currentPosition.second<<")"<<std::endl;
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
    std::cout <<newVehicle->type <<" of "<<newVehicle->width<<" added"<<std::endl;

}
void Road::error_callback(std::string errormsg){
  std::cout <<"[ ERROR ] - "<< errormsg << std::endl;
  std::exit(1);
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
        this->error_callback("Signal can only be GREEN/RED");
    }
}

void Road::updateSim(double delT){


  // Update unrestricted positions
  this->updateUnrestrictedpositions(delT);

  // Update positions of each car
  for(int i=0;i<this->vehicles.size();i++) {
    if(vehicles[i]->isOnRoad && !vehicles[i]->processed){
      vehicles[i]->delT = delT;
      vehicles[i]->updatePos(true);
    }
  }
  this->printLanes();
}

// Runs the simulation and renders the road
void Road::runSim(double delT) {
    this->engine.render(delT);
}


void Road::initLanes(int lanes){
  this->lanes = lanes;
  std::vector<Vehicle*> tmp;
  // Initialize empty lanes;
  std::vector< std::pair<std::vector <Vehicle* >, double > > newtmp(this->lanes,std::make_pair(tmp,0)); // initial backend is 0 for all;
  this->laneVehicles = newtmp;
}

double findLast(std::vector<Vehicle*> vehicles){
  double pos = 0;
  for(auto v: vehicles){
    if((v->currentPosition.first - v->length) < pos){
      pos = (v->currentPosition.first - v->length);
    }
  }
  return pos;
};

void Road::addtoLanes(Vehicle* vehicle,int numlanesreq,int toplane){
  for(int i=0;  i < numlanesreq; i++ ){
    this->laneVehicles[toplane+i].first.push_back(vehicle);
    this->laneVehicles[toplane+i].second -= vehicle->length;
  }
  vehicle->currentLane.first = toplane;
  vehicle->currentLane.second = toplane + numlanesreq - 1;
  std::cout << vehicle->type<<" at lane "<<toplane<<" - "<<toplane + numlanesreq - 1<<std::endl;
}

// top is lane 0
std::pair<double,double> Road::initPosition(Vehicle* vehicle){
  int numlanesreq = std::ceil(vehicle->width*(double)this->lanes / (this->width));
  int lane,positionx=-999;
  if(this->laneVehicles.size()<1){
    this->error_callback("No Lanes are present! (laneVehicles Vector wasn't initialized properly)");
  }
  if(numlanesreq > this->lanes){
    this->error_callback("Vehicle can't be placed on the road! (TOO WIDE)");
  }
  for(int i=0;i+numlanesreq <= this->laneVehicles.size();i++){
    double back=0;
    for(int j=0;j<numlanesreq;j++){
      if(this->laneVehicles[i+j].second < back ){
        back = this->laneVehicles[i+j].second;
      };
    }
    if(back <= 0 && back > positionx){
      positionx = back;
      lane = i;
      if(positionx == 0){
        break;
      }
    }
  }
  this->addtoLanes(vehicle, numlanesreq, lane);
  return std::make_pair(positionx,(this->lanes-lane)*(this->width/(double)this->lanes));
}

bool Road::hasSpace(std::vector<Vehicle*> laneVehicles,double front,double back){
  std::cout << "Checking for space"<<std::endl;
  for(auto v: laneVehicles){
    if(v->currentPosition.first >= back)
      return false;
    if(v->currentPosition.first - v->length <= front)
      return false;
  }
  return true;
}
void Road::changeLane(Vehicle* vehicle){
  // =================
  //       LEFT       0
  //       ----       1
  //       vvvv->     2
  //       ----       3
  //       RIGHT      4
  // =================
  std::cout << "Current "<<vehicle->type<<" at "<<vehicle->currentLane.first<<" "<<vehicle->currentLane.second<<std::endl;
  bool checkRight = true, checkLeft = true;
  if(vehicle->currentLane.first == 0){
    checkLeft = false;
  }
  if(vehicle->currentLane.second == this->lanes - 1){
    checkRight = false;
  }
  std::cout<<"checkLeft:"<<checkLeft<<" checkRight:"<<checkRight<<std::endl;
  if(checkLeft){

    if(this->hasSpace(this->laneVehicles[vehicle->currentLane.first - 1].first, vehicle->currentPosition.first, vehicle->currentPosition.first - vehicle->length))
      {this->updateLane(vehicle->currentLane.first -1, vehicle);vehicle->currentPosition.second+=(this->width/(double)this->lanes);return;} // go back after
  }
  if(checkRight){
    if(this->hasSpace(this->laneVehicles[vehicle->currentLane.second + 1].first, vehicle->currentPosition.first, vehicle->currentPosition.first - vehicle->length))
      {this->updateLane(vehicle->currentLane.second + 1, vehicle);vehicle->currentPosition.second-=(this->width/(double)this->lanes);return;} // go back after
  }
}

void Road::printLanes(){
  for(auto lane : this->laneVehicles){
    for(auto v : lane.first){
      std::cout <<"("<< v->type <<","<<v->currentPosition.first<<");";
    }
    std::cout << "end="<<lane.second<<std::endl;
  }
}

void Road::updateLane(int lane,Vehicle* v){
  std::cout <<"Updating Lane to "<<lane << std::endl;
  // no change for intermediate lanes;
  if(lane < v->currentLane.first){ // shifting to left
    std::cout << "Shifting "<<v->type<<" to Left"<<std::endl;
  int index = -1;
  for(int i=0;i< this->laneVehicles[lane].first.size();i++){
    Vehicle* tmpv = this->laneVehicles[lane].first[i];
    if(tmpv->currentPosition.first >= v->currentPosition.first){
      index = i;
    }
  }
  this->laneVehicles[lane].first.insert(this->laneVehicles[lane].first.begin()+index+1,v);
  v->currentLane.first = lane;
  if(this->laneVehicles[lane].second > v->currentPosition.first - v->length){ // update
    this->laneVehicles[lane].second = v->currentPosition.first - v->length;
  }
  this->removeFromLane(v->currentLane.second,v);
  v->currentLane.second--;
}
  else{
    std::cout << "Shifting "<<v->type<<" to Right"<<std::endl;

  int index = -1;
  for(int i=0;i<this->laneVehicles[lane].first.size();i++){
    Vehicle* tmpv = this->laneVehicles[lane].first[i];
    if(tmpv->currentPosition.first >= v->currentPosition.first){
      index = i;
    }
  }
  this->laneVehicles[lane].first.insert(this->laneVehicles[lane].first.begin()+index+1,v);
    v->currentLane.second = lane;
    if(this->laneVehicles[lane].second > v->currentPosition.first - v->length){ // update
      this->laneVehicles[lane].second = v->currentPosition.first - v->length;
    }
    this->removeFromLane(v->currentLane.first,v);
    v->currentLane.first++;
  }
}
void Road::removeFromLane(int lane,Vehicle* v){
    double back = this->length;int index=0;
    if(this->laneVehicles[lane].first.size()<1){
      this->error_callback("Lane Size shouldn\'t be zero!");
    }
    for(int i=0;i<this->laneVehicles[lane].first.size();i++){
      Vehicle* tmpv = this->laneVehicles[lane].first[i];
      if(tmpv==v){
        index = i;
        continue;
      }
      if(tmpv->currentPosition.first - tmpv->length < back){
        back = tmpv->currentPosition.first - tmpv->length;
      }
    }
    this->laneVehicles[lane].second = back;
    this->laneVehicles[lane].first.erase(this->laneVehicles[lane].first.begin() + index);
}
// Gives first obstacle position in the given window
double Road::firstObstacle(Vehicle* vehicle) {
    double position=9999;
    #ifndef NEW
    for(auto laneinfo: this->laneVehicles){
      if(std::find(laneinfo.first.begin(), laneinfo.first.end(), vehicle) != laneinfo.first.end()){
        Vehicle* lastV=NULL;
        for(auto v : laneinfo.first){
          if(v->isOnRoad){
          if(v == vehicle){
            break;
          }
          else{
            if(!v->processed){
              v->updatePos(true);
            }
            lastV = v; // get the last element
          }}
        }
        if(lastV != NULL){
          std::cout << "last for "<<  vehicle->type << "="<<lastV->type<<std::endl;
          if( position > (lastV ->currentPosition.first - lastV->length) )
            position = lastV ->currentPosition.first - lastV->length;
          std::cout << "OBSTACLE == "<<position<<std::endl;
        }
      }
    }
    #else
    for(auto v : this->vehicles) {
        if((v->unrestrictedposition.second < topRow && (v->unrestrictedposition.second)>botRow) || ((v->unrestrictedposition.second-v->width)>botRow) && (v->unrestrictedposition.second-v->width) < topRow){
            double back = (v->unrestrictedposition.first-v->length);
            if(position > back && back > startPos ){
                position = back;
            }
        }
    }
    #endif
    return position;
}

// Updates the unrestricted new positions of every vehicle
void Road::updateUnrestrictedpositions(double delT){

  for(auto v : this -> vehicles) {
    if(v->isOnRoad)
      { v->delT=delT;
        v->updatePos(false);}
  };
  // std::cout <<"Updated unrestricted positions for everyone" <<std::endl;

}

bool Road::isRed() {
    return (!this->signal.compare("RED"));
}
