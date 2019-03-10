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
  // OpenGL part
  // Default window lengths and widths
  this->window_length = 640;
  this->window_height = 480;
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
    Vehicle newVehicle = *vehicle; // Make a copy from vehicle template
    newVehicle.setColor(color);
    newVehicle.isOnRoad = true;
    newVehicle.parentRoad = this;
    newVehicle.currentPosition = this->initPosition();
    this->vehicles.push_back(&newVehicle);
    // Add the road to the vehicle

    // To set defaults of road if not constructed
    vehicles.back()->reConstruct();
}

void Road::setSignal(std::string signal){
  if(!signal.compare("GREEN")){
    this->signal = signal;
    this->signal_rgb[0] = 11;
    this->signal_rgb[1] = 229;
    this->signal_rgb[2] = 8;
  }
  if(!signal.compare("RED")){
    this->signal = signal;
    this->signal_rgb[0] = 237;
    this->signal_rgb[1] = 32;
    this->signal_rgb[2] = 32;
  }
  {
    std::cout<<"[ ERROR ] Signal can only be GREEN/RED";
  }
}

// Runs the simulation and renders the road
void Road::runSim(double delT) {
    // Run until time is exhausted
    double beginTime =
    #ifdef RENDER_ENGINE_H
    this->engine.getTime();
    #else
    0;
    #endif
    double oldTime;
    double currentTime =
    #ifdef RENDER_ENGINE_H
    this->engine.getTime();
    #else
    0;
    #endif

    while(currentTime - beginTime < delT) {

        this->updateUnrestrictedpositions(currentTime - oldTime);
        // Update positions of each car
        for(int i=0;i<this->vehicles.size();i++) {
            vehicles[i]->updatePos(currentTime - oldTime,true);
        }
        oldTime = currentTime;
        currentTime =
        #ifdef RENDER_ENGINE_H
        this->engine.getTime();
        #else
        0.1;
        #endif
        // Render the current state
        #ifdef RENDER_ENGINE_H
        this->engine.render();
        #endif
    }
}

std::pair<double,double> Road::initPosition(){

  // complex algorithm later
  // std::vector<std::pair < std::pair<double,double>, double> > map; // ((top,bot),minposX)
  double posx=this->length ;//, posy=this->width;
  // std::vector< std::pair< double, double > > obs;
  // posX is x co-ordinate on road, posY is y co-ordinate
  // for(int i=0;i<this->vehicles.size();i++){
  //   // Vehicle* v = vehicles[i];
  //   // if(map.length == 0){
  //   //   map.push_back(std::make_pair(v->currentPosition.second,v->currentPosition.first - v->length));
  //   //   map.push_back(std::make_pair(v->currentPosition-v->width,v->currentPosition.first - v->length));
  //   // }
  //   // traverse through map;
  //   // for(int k = 0;k<map.size();k++){
  //   //
  //   // }
  // }
  for(int i=0;i<this->vehicles.size();i++){
    if((vehicles[i]->currentPosition.first - vehicles[i]->length) < posx ) // backEnd of vehicle
    {
      posx = (vehicles[i]->currentPosition.first - vehicles[i]->length);
      // posy = vehicles[i]->currentPosition.second;
    }
}
  if(posx>0){
    return std::make_pair(0,this->width);
  }
  else
    return std::make_pair(posx,this->width);
  // obs.push_back(make_pair(posx,posy));

}
// std::vector<std::pair<double ,double> > Road::topedgeprofile(){
//   std::vector<std::pair<double ,double> > tmp;
// }
// std::vector<std::pair<double ,double> > Road::botedgeprofile(){
//
// }
// Gives first obstacle position in the given window
double Road::firstObstacle(double startPos, double topRow, double botRow ){
  double position=this->length;
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
    v->updatePos(delT,false);
  };
}
// The error_callback function prints out the error and exits with non-zero status
// static void Road::error_callback(int error, const char* description) {
//     std::cerr << description << std::endl;
//     std::exit(1);
// }

//
// void Road::setupRoad() {
//     // Initialize GLFW
//     glfwInit();
//     glEnable(GL_DEPTH_TEST);
//     if (!glfwInit()) {
//         exit(EXIT_FAILURE);
//     }
//
//     // Set the error_callback function
//     glfwSetErrorCallback(Road::error_callback);
//
//     // Create a new window
//     this->window = glfwCreateWindow(this->window_length, this->window_height, "SimView", NULL, NULL);
//     if (!this->window) {
//         glfwTerminate();
//         exit(EXIT_FAILURE);
//     }
//
//     // Make this context current
//     glfwMakeContextCurrent(this->window);
//
//     // Set the key_callback function
//     glfwSetKeyCallback(window, Road::key_callback);
// }
//
// void Road::renderRoad() {
//     // Get frameBuffer attributes
//     double ratio;
//     int frame_height, frame_width;
//     glfwGetFramebufferSize(this->window, &window_height, &window_width);
//     ratio = frame_width/(double)frame_height;
//
//     // Create a blank viewport
//     glViewport(0, 0, width, height);
//     glClear(GL_COLOR_BUFFER_BIT);
//
//     // Render the road as a rectangle
//
//
//     // Swap the buffers, to display rendered stuff on the screen
//     glfwSwapBuffers(this->window);
//     glfwPollEvents();
// }
int main(){
    std::cout<<"compiles EXIT_SUCCESS"<<std::endl;
}
