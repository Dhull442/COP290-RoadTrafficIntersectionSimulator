#include <bits/stdc++.h>
#include "Vehicle.h"
#include "Road.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include "RenderEngine.h"


#define ANSI_COLOR_RED     "\033[1;31m"
#define ANSI_COLOR_GREEN   "\033[1;32m"
#define ANSI_COLOR_YELLOW  "\033[1;33m"
#define ANSI_COLOR_BLUE    "\033[1;34m"
#define ANSI_COLOR_MAGENTA "\033[1;35m"
#define ANSI_COLOR_CYAN    "\033[1;36m"
#define ANSI_COLOR_WHITE   "\033[1;37m"
#define ANSI_COLOR_BLACK   "\033[1;1m"
#define ANSI_COLOR_RESET   "\033[0m"

RenderEngine::RenderEngine(Road* targetRoad) {
    std::cout << "Instantiated RenderEngine for road " << targetRoad->id << std::endl;
    this->targetRoad = targetRoad;
    // Dividing by this factor gives it in viewport dimensions
    this->scalex = 25;
    this->scaley = 50;
    this->signalSize = 1;
    // Set the default background color
    this->bgcolor.push_back(1.0f);
    this->bgcolor.push_back(0.968f);
    this->bgcolor.push_back(0.3529f);
    this->monitorWidth = 800;
    this->monitorHeight = 600;
}

// Default constructor
RenderEngine::RenderEngine() {
    // Do nothing
    // std::cout << "RenderEngine() default constructor called" << std::endl;
}

void RenderEngine::error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

// Function which receives the key events
void RenderEngine::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        // Set the close_flag to true
        std::cout << "Exit keypress" << std::endl;
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

// Initalize GLFW stuff
void RenderEngine::setup() {
    std::cout << "Setting up the RenderEngine" << std::endl;
    // Initialize GLFW, return error otherwise
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    // Set the error_callback function
    glfwSetErrorCallback(RenderEngine::error_callback);

    // Create a window, context
    std::cout << "Creating a window" << std::endl;
    RenderEngine::window = glfwCreateWindow(this->monitorWidth, this->monitorHeight, "TrafficSim", NULL, NULL);

    if (!RenderEngine::window) {
        // Context creation failed
        glfwTerminate();
        std::cout << "Could not create window" << std::endl;
        exit(EXIT_FAILURE);
    }


    RenderEngine::isInitialized = true;
    // Set framerate to 25
    this->fps = 25;
    // Make the context current
    glfwMakeContextCurrent(RenderEngine::window);
    // Set the swap interval
    glfwSwapInterval(1);
    // Set the key_callback method
    glfwSetKeyCallback(RenderEngine::window, RenderEngine::key_callback);
}

float RenderEngine::getTime() {
    float time = glfwGetTime();
    return time;
}

void RenderEngine::render(double delT) {
    double beginTime = RenderEngine::getTime();
    double oldTime = RenderEngine::getTime();
    double currentTime = RenderEngine::getTime();
    std::cout << "Starting Render routine"<< std::endl;
    bool update = false;
    while((currentTime - beginTime < delT) && !glfwWindowShouldClose(RenderEngine::window)) {
    	if (currentTime - oldTime >= 1/fps) {
        	// Update the simulation based on previously decided parameters, set new parameters
        	this->targetRoad->updateSim(currentTime - oldTime, RenderEngine::getTime());
    		update = true;
    	}

        float ratio;
        int width=800, height=800;
        glfwGetFramebufferSize(RenderEngine::window, &width, &height);

        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render the background
        glClearColor((float)this->bgcolor[0], (float)this->bgcolor[1], (float)this->bgcolor[2], 1.0f);
        // Render the road
        RenderEngine::renderRoad();

        // Iterate over the vehicles
        for(auto v: this->targetRoad->vehicles) {
            renderVehicle(v);
        }

        // Swap buffers and check for events
        glfwSwapBuffers(RenderEngine::window);
        glfwPollEvents();
	if (update) {
    {this->generateMap();
    this->renderMap();}
		update = false;
        	oldTime = currentTime;
	}
        currentTime = RenderEngine::getTime();
    }
}

void RenderEngine::initializeMap(){
  // this->fout.open("output.txt");
  std::vector< std::pair<char,std::string> > tmp((int)this->targetRoad->length,std::make_pair(' ',ANSI_COLOR_BLACK));
  std::vector< std::vector <std::pair<char,std::string> > > tmp2((int)this->targetRoad->width, tmp);
  this->map = tmp2;
}

void RenderEngine::renderMap(){
  std::ofstream fout("output.txt", std::ios_base::app);
  if(this->map.size()<1){
    std::cout << "[ ERROR ] - Map was not initialized properly!"<<std::endl;
    std::exit(1);
  }
  for(int i=0;i<this->map[0].size();i++){ // print top boundary
    fout << ANSI_COLOR_YELLOW <<"=" << ANSI_COLOR_RESET;
    if(i == (int)this->targetRoad->signalPosition ){
      fout<< this->targetRoad->ascii_signalcolor << "|" << ANSI_COLOR_RESET;
    }
    else
      fout << " ";
  }
  fout << "\n";;

  for(int i=0;i<this->map.size();i++){
    for(int j = 0 ; j < this->map[i].size();j++){
      fout << this->map[i][j].second << this->map[i][j].first << ANSI_COLOR_RESET;
      if(j == (int)this->targetRoad->signalPosition ){
        fout<< this->targetRoad->ascii_signalcolor << "|" << ANSI_COLOR_RESET;
      }
      else
        fout << " ";
    }
    fout << "\n";
  }

  for(int i=0;i<this->map[0].size();i++){ // print bottom boundary
    fout << ANSI_COLOR_YELLOW <<"=" << ANSI_COLOR_RESET;
    if(i == (int)this->targetRoad->signalPosition ){
      fout<< this->targetRoad->ascii_signalcolor << "|" << ANSI_COLOR_RESET;
    }
    else
      fout << " ";
  }
  fout <<"\n\n\n";;
}

void RenderEngine::generateMap(){
  // Refresh Everything
  for(int i=0;i<this->map.size();i++){
    for(int j=0;j<this->map[0].size();j++){
      this->map[i][j].first = ' ';
    }
  }
  for(auto v: this->targetRoad->vehicles){
    for(int i=(int)(v->currentPosition.first-v->length); i<(int)v->currentPosition.first; i++){
      if(i>=0 && i< this->map[0].size()){
        for(int j = (int)(v->currentPosition.second - v->width); j < (int) (v->currentPosition.second); j++){
          if(j>0 && j <= this->map.size()){
            this->map[this->map.size()-j][i].first = v->type[0];
            this->map[this->map.size()-j][i].second = v->ascii_color;
          }
        }
      }
    }
  }
}

void RenderEngine::renderRoad() {
    // Render the road in gray
    glColor3f(0.2f, 0.2f, 0.2f);
    float ycoord = this->targetRoad->width/((float)this->scaley);
    float xcoord = this->targetRoad->length/(float)this->scalex - 1.0;
    glRectd(-1.0f, ycoord, xcoord, -ycoord);

    // Render the signal as a strip
    float xsignal = this->targetRoad->signalPosition/(float)this->scalex - 1.0;
    glColor3f((float)this->targetRoad->signal_rgb[0]/255.0f, (float)this->targetRoad->signal_rgb[1]/255.0f, (float)this->targetRoad->signal_rgb[2]/255.0f);
    glRectd(xsignal, ycoord, xsignal + this->signalSize/(float)this->scalex, -ycoord);
    int lanes = this->targetRoad->lanes;
    double lanewidth = this->targetRoad->width/(double)this->targetRoad->lanes;
    for(int i=0;i<lanes-1;i++){
      glColor3f(1.0f,1.0f,1.0f);
      float ystart = -ycoord + 2*(i+1)*lanewidth/(float)this->scaley;
      float width = 0.01f;
      glRectd(-1.0f, ystart, xcoord, ystart + width);
    }
}

void RenderEngine::endSim() {
    glfwTerminate();
}

void RenderEngine::renderVehicle(Vehicle* vehicle) {
    if (vehicle->isOnRoad) {
        // Render only if the vehicle is on the Road
        float x = -1.0 + (float)vehicle->currentPosition.first/(float)this->scalex;
        float y = 2*( - (float)this->targetRoad->width/2 + (float)vehicle->currentPosition.second)/(this->scaley);
        float delx = vehicle->length/(float)this->scalex;
        float dely = 2*vehicle->width/(float)this->scaley;

        // Set the correct color
        glColor3f((float)vehicle->color_rgb[0]/255.0f,
        (float)vehicle->color_rgb[1]/255.0f,
        (float)vehicle->color_rgb[2]/255.0f);

        // Render the rectangle
        // std::cout << "Vehiclewa "<<vehicle->type<<" "<<vehicle->width<<" "<<delx << " " <<dely << std::endl;
        glRectd(x, y, x -  delx, y - dely);
    }
}
