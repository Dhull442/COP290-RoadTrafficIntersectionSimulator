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
    newVehicle->currentPosition = this->initPosition2(newVehicle);
    std::cout << "Position intialized to ("<<newVehicle->currentPosition.first<<","<<newVehicle->currentPosition.second<<")"<<std::endl;
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
}

// top is lane 0
std::pair<double,double> Road::initPosition2(Vehicle* vehicle){
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

// Not useful anymore
std::pair<double,double> Road::initPosition(Vehicle* vehicle){
  // Lane implementation
  double lanewidth = this->width / (double) this->lanes;
  double lanepos=this->width;
  int laneno=this->lanes - 1;
  double posy=this->width,posx=-999;
  for( int i=0;laneno >= 0; ){
      if( i >= this->vehicles.size()){
          {
          posx = 0;
          posy = (laneno+1) * lanewidth;
          }
          break;
      }
      else{
          double localposx=0,localposy=0;
          while(i<vehicles.size()){
                Vehicle* v = this->vehicles[i];
            if(v->currentPosition.second <= lanewidth*laneno){
              // std::cout <<"shiftin to search in new lane "<<laneno-1<<std::endl;
              laneno--;
              break;
            }
            double back = (v->currentPosition.first-v->length);
            // std::cout<<"Back at "<<back<<" in lane "<<laneno<<std::endl;
            if(back < localposx){
                // std::cout <<"POS = "<<back<<std::endl;
                localposx = back;
            }
            localposy = (laneno + 1) * lanewidth;
          i++;
        }
        if(i == vehicles.size() && localposx < 0){
          // std::cout <<"lane num decreased because it can"<<std::endl;
          laneno--;
        }
        if(posx < localposx){
          // std::cout<<"setting pos from localpos "<<localposx<<" "<<posx<<std::endl;
          posx = localposx;
          posy = localposy;
        }
    }
    if(posx == 0){
      break;

    }
  }
  end:
  return std::make_pair(posx,posy);
}
// take care of how vehicles go into sort order into lane vehicles array ;
// Gives first obstacle position in the given window
double Road::firstObstacle(Vehicle* vehicle) {
    double position=this->length+2*length;
    #ifndef NEW
    for(auto laneinfo: this->laneVehicles){
      if(std::find(laneinfo.first.begin(), laneinfo.first.end(), vehicle) != laneinfo.first.end()){
        Vehicle* lastV=NULL;
        for(auto v : laneinfo.first){
          if(v == vehicle){
            break;
          }
          else{
            if(!v->processed){
              v->updatePos(true);
            }
            lastV = v; // get the last element
          }
        }
        if(lastV != NULL){
          if( position > (lastV ->currentPosition.first - lastV->length) )
            position = lastV ->currentPosition.first - lastV->length;
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
