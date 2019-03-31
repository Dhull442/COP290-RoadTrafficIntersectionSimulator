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

void Road::setDefaults(double maxspeed, double acceleration,double length, double width,int skill,double sdistance){
    // Input correctly < No checks here >
    this->default_maxspeed = maxspeed;
    this->default_acceleration = acceleration;
    this->default_length = length;
    this->default_width = width;
    this->default_skill = skill;
    this->default_safety_distance = sdistance;
    // ADD TO CONFIG FILE LATER
    this->sideClearance = 0.4;
}

// For adding vehicle
void Road::addVehicle(Vehicle* vehicle,std::string color) {
    // Vehicle from template
    // Make a copy from the Vehicle template
    Vehicle* newVehicle = new Vehicle(*vehicle);
    newVehicle->isOnRoad = true;
    newVehicle->parentRoad = this;
    
    newVehicle->setColor(color);
    // Set the new acceleration and velocities to zero
    newVehicle->a = 0;
    newVehicle->currentSpeed = 0;
    // Constructs the parameters of the vehicle from either the template or the defaults
    newVehicle->reConstruct();
    newVehicle->currentPosition = this->initPosition(newVehicle);
    newVehicle->changingLane = false;
    newVehicle->speedRatio = 3;
    newVehicle->lastLaneChange = -100;
    newVehicle->timeGap = 2;
    newVehicle->verticalSpeed = 0;
    newVehicle->changeDirection = 1;
    newVehicle->verticalSpeed = 0;
    // Pushes into the vector of vehicles sorted by position
    if(this->vehicles.size() < 1){
      this->vehicles.push_back(newVehicle);
    } else {
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
    std::cout << newVehicle->type << " of " << newVehicle->width<<" added"<< " with positionx " << newVehicle->currentPosition.first << std::endl;
}

void Road::error_callback(std::string errormsg){
  std::cout <<"[ ERROR ] - "<< errormsg << std::endl;
  std::exit(1);
}

// Set the signal color, and value
void Road::setSignal(std::string signal){
    if(!signal.compare("GREEN")){
        this->signal = signal;
        this->signal_rgb[0] = 11;
        this->signal_rgb[1] = 229;
        this->signal_rgb[2] = 8;
        return;
    }

    if(!signal.compare("RED")) {
        this->signal = signal;
        this->signal_rgb[0] = 237;
        this->signal_rgb[1] = 32;
        this->signal_rgb[2] = 32;
        return;
    } else {
        this->error_callback("Signal can only be GREEN/RED");
    }
}

void Road::updateSim(double delT, double globalTime){
    // Make the processed variable false for all car
    for(int i = 0; i < this->vehicles.size(); i++) {
        vehicles[i]->processed = false;
    }

    // Update positions of each car
    for(int i=0;i<this->vehicles.size();i++) {
      if(vehicles[i]->isOnRoad && !vehicles[i]->processed){
          // Update positions based on previous parameters and update parametersin 
          std::cout << "Updating from the main function " << std::endl;
          vehicles[i]->updatePos(delT, globalTime);
          // this->printLanes();
      }
    }

    this->printLanes();
    std::cout << "NOW CALLING CHANGE LANES FROM THE MAIN FUNC" << std::endl;

    for(int i=0; i < this->vehicles.size(); i++) {
        vehicles[i]->changeLane(delT, globalTime);
    }
    this->printLanes();
}

// Runs the simulation and renders the road
void Road::runSim(double delT) {
    this->engine.render(delT);
}

// Initializes empty Lanes
void Road::initLanes(int lanes){
  this->lanes = lanes;
  std::vector<Vehicle*> tmp;
  // Initialize empty lanes;
  std::vector< std::vector <Vehicle* > > newtmp(this->lanes, tmp);
  this->laneVehicles = newtmp;
}

// Finds the vehicle the most back on the road
double findLast(std::vector<Vehicle*> vehicles){
  double pos = 0;
  for(auto v: vehicles){
    if((v->currentPosition.first - v->length) < pos){
      pos = (v->currentPosition.first - v->length);
    }
  }
  return pos;
};

// Add the Vehicle to all lanes; right at the end of each one
void Road::addtoLanes(Vehicle* vehicle, int numlanesreq, int toplane){
  for(int i=0;  i < numlanesreq; i++ ) {
    this->laneVehicles[toplane+i].push_back(vehicle);
  }

  vehicle->currentLane.first = toplane;
  vehicle->currentLane.second = toplane + numlanesreq - 1;
}

// A NULL indicates, there is nothing or just the signal
bool Road::getAdjVehicles(Vehicle* vehicle, int dir, double delT, double globalTime) {
  int laneno;
  double frontPos = vehicle->currentPosition.first;
  double backPos = frontPos - vehicle->length;
  
  if (dir == 1) {
    laneno = vehicle->currentLane.second+1;
  } else {
    laneno = vehicle->currentLane.first-1;
  }

  if (laneno < 0 || laneno >= this->laneVehicles.size()) {
    return false;
  }

  std::cout << "Checking Lanes for " << vehicle->color << " " << vehicle->type << " in " << laneno << std::endl;
  
  if (this->laneVehicles[laneno].size() == 0) {
    std::cout << "Adj lane is empty" << std::endl;
    vehicle->front = NULL;
    vehicle->back = NULL;
    return true;
  }

  if (laneno >= 0 && laneno < this->laneVehicles.size()) {
    // Iterate over the vehicles in this lane

    for(int i = 0; i <= this->laneVehicles[laneno].size(); i++) {
      if (i == 0) {
        // For the first vehicle
        // if (!this->laneVehicles[laneno][0]->processed) {std::cout << "Updating from getAdjVehicles" << std::endl; this->laneVehicles[laneno][0]->updatePos(delT, globalTime);}
        if (this->laneVehicles[laneno][0]->currentPosition.first < backPos) {
          vehicle->front = NULL;
          vehicle->back = this->laneVehicles[laneno][0];
          std::cout << "Found a space at the beginning" << std::endl;
          return true;
        } 
      } else if (i == this->laneVehicles[laneno].size()) {
        Vehicle* lastV = this->laneVehicles[laneno][i-1];
        if (lastV->currentPosition.first-lastV->length > frontPos) {
          vehicle->front = lastV;
          vehicle->back = NULL;
          std::cout << "Found a space at the end" << std::endl;
          return true;
        }
      } else {
        Vehicle* frontVehicle = this->laneVehicles[laneno][i-1];
        Vehicle* backVehicle = this->laneVehicles[laneno][i];
        // if (!frontVehicle->processed) {std::cout << "Updating from getAdjVehicles" << std::endl; frontVehicle->updatePos(delT, globalTime);}
        // if (!backVehicle->processed) {std::cout << "Updating from getAdjVehicles" << std::endl; backVehicle->updatePos(delT, globalTime);}
        if (frontPos < frontVehicle->currentPosition.first-frontVehicle->length && backPos > backVehicle->currentPosition.first) {
          std::cout << "Found a space between " << frontVehicle->color << " " << frontVehicle->type << " " << backVehicle->color << " " << backVehicle->type << std::endl;  
          vehicle->front = frontVehicle;
          vehicle->back = backVehicle;
          return true;
        }
      }
    }
    return false;
  } else {
    return false;
  }
}

// Calculates the back ends of each lane
std::vector<double> Road::calculateBackEnds(){
  std::vector<double> result;
  for(int i=0;i<this->lanes;i++){
    double back = 999;
    for(auto v: this->laneVehicles[i]){
      if(back > v->currentPosition.first - v->length){
        back = v->currentPosition.first - v->length;
      }
    }
    result.push_back(back);
  }
  return result;
}

// Get the position of the vehicle on the road
std::pair<double,double> Road::initPosition(Vehicle* vehicle) {
  std::cout << "Finding a position for this Vehicle" << std::endl;
  int numlanesreq = std::ceil((vehicle->width + 2*this->sideClearance)*(double)this->lanes / (this->width));

  std::cout << "This Vehicle spans " << numlanesreq << " lanes" << std::endl;
  int lane;
  double positionx = -999;

  if( this->laneVehicles.size() < 1){
    this->error_callback("No Lanes are present! (laneVehicles Vector wasn't initialized properly)");
  }

  if(numlanesreq > this->lanes) {
    this->error_callback("Vehicle cant't be placed on the road! (TOO WIDE)");
  }

  // Calculate the back ends of each lane
  std::vector<double> backEnd = this->calculateBackEnds();
  std::cout << "Backends of each lane: " << this->laneVehicles.size() << " "; 
  for(auto x: backEnd) {std::cout << x << ", ";}
  std::cout << std::endl;
  
  // Iterate over the bunch of lanes
  for(int i = 0; i + numlanesreq <= this->laneVehicles.size(); i++) {
    double back=0;
    // Find the final car in this lane
    for(int j = 0; j < numlanesreq; j++) {
      if(backEnd[i+j] < back){
        back = backEnd[i+j];
      }
    }

    // Place in the first available lane from the top
    if(back <= 0 && back > positionx) {
      std::cout << "Update positionx with " << back << std::endl;
      positionx = back;
      lane = i;
    }
  }

  std::cout << "Final position " << positionx << std::endl;
  // Add the vehicle to the lane, at the end of each one
  this->addtoLanes(vehicle, numlanesreq, lane);
  double xcoord = positionx-vehicle->safedistance*2;
  double ycoord = (this->lanes-lane)*(this->width/(double)this->lanes) - this->sideClearance;
  // This return value is assigned to the current position - and a buffer is added
  std::cout << "Final value " << xcoord << ", " << ycoord << std::endl;
  return std::make_pair(xcoord, ycoord);
}

// Checks if there is space on an adjacent Lane -- WILL BE EDITED
bool Road::hasSpace(std::vector<Vehicle*> laneVehicles,double front,double back) {
  return true;
}

// CHANGES THE LANE - WILL BE EDITED
void Road::changeLane(Vehicle* vehicle){}

// Prints the lanes for debugging
void Road::printLanes(){
  int i = 0;
  for(auto lane : this->laneVehicles){
    std::cout << "LANE #" << i << ":"; i++;
    for(auto v : lane){
      std::cout << "(" << v->color << " " << v->type << ", (" << v->currentPosition.first << " " << v->currentPosition.second << "), (" << v->currentSpeed << " " << v->verticalSpeed << "), " << v->closestDistance << "," << v->a << ", " << v->currentLane.first  << " " << v->currentLane.second << " " << v->changingLane << " " << v->delT << ");";
    }
    std::cout<<std::endl;
  }
}

// Updates lanes after shifting -- WILL BE EDITED
void Road::updateLane(int lane,Vehicle* v){
  return;
}

// Find the first obstacle in front of an object in the updated state -- WILL BE EDITED
double Road::firstObstacle(Vehicle* vehicle, double delT, double globalTime) {
    std::cout << "Detecting obstacle for " <<vehicle->color << " " << vehicle->type << " at " << vehicle->currentPosition.first << std::endl;
    // This is the position of the first Obstacle in front
    double position=9999;
    // Cycle over all lane
    for(auto laneinfo: this->laneVehicles) {
      if(std::find(laneinfo.begin(), laneinfo.end(), vehicle) != laneinfo.end()) {
        // If vehicle exists in this lane, execute
        // Pointer to the last vehicle in the lane in front if this one
        Vehicle* lastV = NULL;
        // Iterate over the vehicles in the lanes
        for(auto v : laneinfo) {
              if(v->isOnRoad) {
                  if(v == vehicle) {
                    // If we reach this vehicle, break out
                    break;
                  } else {
                    if (!v->processed) {
                        // If this Vehicle is not processed, update the Positions
                        std::cout << "Updating from firstObstacle" << std::endl;
                        v->updatePos(delT, globalTime);
                        std::cout << "Updated " << v->processed << std::endl;
                        // Road::printLanes();
                    }
                     
                    // Get the last element in
                    lastV = v;
                 }
             }
        }

        // After the loop
        if(lastV != NULL && position > lastV->currentPosition.first - lastV->length) {
            // There is some Vehicle in the front of this one, in current lane
            std::cout << "last for "<< vehicle->color << " " << vehicle->type << " = " << lastV->color << " " << lastV->type << std::endl;
            position = lastV->currentPosition.first - lastV->length;
            std::cout << "OBSTACLE == " << position << std::endl;
        } else {
            // Check the signal position, if signal is RED
            if (position > this->signalPosition && this->signal.compare("RED") == 0) {
                position = this->signalPosition;
            }
        }
      }
    }
    double result = position - vehicle->currentPosition.first;
    std::cout << "The position of THIS vehicle is " << vehicle->currentPosition.first << std::endl;
    std::cout << "The final result value is " << result << std::endl;
    return result;
}

bool Road::isRed() {
  if (this->signal.compare("RED") == 0) {
    return true;
  } else {
    return false;
  }
}

void Road::removeFromLane(Vehicle* v, int laneno) {
  std::cout << "REMOVE CALL" << laneno << std::endl;
  std::vector<Vehicle*> newLane;
  std::vector<Vehicle*>::iterator it = this->laneVehicles[laneno].begin();
  while(it != this->laneVehicles[laneno].end()) {
    if (*it != v) {
      newLane.push_back(*it);
    }
    it++;
  }
  this->laneVehicles[laneno].clear();
  for(auto v: newLane) {
    this->laneVehicles[laneno].push_back(v);
  }
}

void Road::insertInLane(Vehicle* front, int laneno, Vehicle* v) {
  std::cout << "INSERT CALL" << std::endl;
  std::vector<Vehicle*>::iterator it = this->laneVehicles[laneno].begin();
  std::vector<Vehicle*> newLane;

  if (front == NULL) {
    std::cout << "Insert at the beginning" << std::endl;
    newLane.push_back(v);

    while(it != this->laneVehicles[laneno].end()) {
      std::cout << "Pushing " << (*it)->color << ", " << (*it)->type << std::endl;
      newLane.push_back(*it);
      it++;
    }

    this->laneVehicles[laneno].clear();
    for(auto v: newLane) {this->laneVehicles[laneno].push_back(v);}
    std::cout << "Exiting after insertion" << std::endl;
    return;
  }

  while(it != this->laneVehicles[laneno].end()) {
    if (*it = front) {
      std::cout << "Inserting in between" << std::endl;
      newLane.push_back(*it);
      newLane.push_back(v);
      it++;
    } else {
      newLane.push_back(*it);
      it++;
    }
  }
  this->laneVehicles[laneno] = newLane;
}