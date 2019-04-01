#include <bits/stdc++.h>
#include "Vehicle.h"
#include "Road.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstdio>
#include "Render.h"

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
    // Set framerate to 25
    this->fps = 100;
    // Set the default background color
    this->bgcolor.push_back(0.701f);
    this->bgcolor.push_back(0.341f);
    this->bgcolor.push_back(0.1f);
    this->monitorWidth = 1024;
    this->monitorHeight = 620;
    this->isInitialized = false;
    this->CamX = 0;
    this->CamY = 0;
    this->CamAngleX=0;
    this->CamAngleY=0;
    this->CamZoom = -20;


    this->setCameraSpeed(1.0f,1.0f,1.0f); // Change defaults according to need
    this->initializeModels();
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
    if(!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return;
    }
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing

    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow(this->monitorWidth, this->monitorHeight, "ROAD TRAFFIC SIMULATOR", NULL, NULL);

    if(window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    // Set the swap interval
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, this->key_callback);

    // Get info of GPU and supported OpenGL version
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST); // Depth Testing
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    this->window = window;
    this->isInitialized = true;
    std::cout << this->map.size() << std::endl;
}

void RenderEngine::initializeMap(){
  // this->fout.open("output.txt");
  std::vector< std::pair<char,std::string> > tmp((int)this->targetRoad->length,std::make_pair(' ',ANSI_COLOR_BLACK));
  std::vector< std::vector <std::pair<char,std::string> > > tmp2((int)this->targetRoad->width, tmp);
  this->map = tmp2;
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
        // std::cout << "Rendering now..." << std::endl;
        // std::cout << currentTime - oldTime << " >= "<< 1/fps << std::endl;
        if(currentTime - oldTime >= 1/fps){
        	// Update the simulation based on previously decided parameters, set new parameters
        	this->targetRoad->updateSim(currentTime - oldTime, RenderEngine::getTime());
          update = true;
    	  }
        this->UpdateCamera(currentTime - beginTime );


        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        RenderEngine::renderRoad();
        // Iterate over the vehicles
        for(auto v: this->targetRoad->vehicles) {
            RenderEngine::renderVehicle(v);
        }
        /* Cleanup states */
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

        // Swap buffers and check for events
        glfwSwapBuffers(RenderEngine::window);
        glfwPollEvents();

        if (update) {
          // if((int)1000*((currentTime-beginTime)-floor(currentTime-beginTime))  < 100)
          {this->generateMap();
          this->renderMap();}
      		update = false;
          oldTime = currentTime;
      	}
        currentTime = RenderEngine::getTime();
    }
}

void RenderEngine::setCameraSpeed(float translationspeed, float rotationspeed, float zoomspeed){
  this->CamTranslationSpeed = translationspeed;
  this->CamRotationSpeed = rotationspeed;
  this->CamZoomSpeed = zoomspeed;
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
  std::cout << "Generating Map"<<std::endl;
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
void RenderEngine::UpdateCamera(double delT){
  GLint windowWidth, windowHeight;
  glfwGetWindowSize(window, &windowWidth, &windowHeight);
  glViewport(0, 0, windowWidth, windowHeight);
  // Draw stuff
  glClearColor(this->bgcolor[0], this->bgcolor[1], this->bgcolor[2],0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION_MATRIX);
  glLoadIdentity();
  gluPerspective( 90, (double)windowWidth / (double)windowHeight, 0.1, 100 );
  if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
    this->CamY -= this->CamTranslationSpeed * delT;
  }
  // Move backward
  if (glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS){
    this->CamY += this->CamTranslationSpeed * delT;
  }
  // Strafe right
  if (glfwGetKey(window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
    this->CamX -= this->CamTranslationSpeed * delT;
  }
  // Strafe left
  if (glfwGetKey(window, GLFW_KEY_LEFT ) == GLFW_PRESS){
    this->CamX += this->CamTranslationSpeed * delT;
  }
  if (glfwGetKey(window, GLFW_KEY_W ) == GLFW_PRESS){
    this->CamAngleX += this->CamRotationSpeed * delT;
  }
  if (glfwGetKey(window, GLFW_KEY_S ) == GLFW_PRESS){
    this->CamAngleX -= this->CamRotationSpeed * delT;
  }
  if (glfwGetKey(window, GLFW_KEY_A ) == GLFW_PRESS){
    this->CamAngleY += this->CamRotationSpeed * delT;
  }
  if (glfwGetKey(window, GLFW_KEY_D ) == GLFW_PRESS){
    this->CamAngleY -= this->CamRotationSpeed * delT;
  }
  if (glfwGetKey(window, GLFW_KEY_Z ) == GLFW_PRESS){
    this->CamZoom += this->CamZoomSpeed * delT;
  }
  if (glfwGetKey(window, GLFW_KEY_X ) == GLFW_PRESS){
    this->CamZoom -= this->CamZoomSpeed * delT;
  }
  if (glfwGetKey(window, GLFW_KEY_R ) == GLFW_PRESS){
    this->CamZoom = -20;
    this->CamAngleX = 0;
    this->CamAngleY = 0;
    this->CamX = 0;
    this->CamY = 0;
  }
  double angle = sqrt(pow(this->CamAngleX,2)+pow(this->CamAngleY,2));
  glTranslatef(this->CamX,this->CamY,this->CamZoom);
  glRotatef(angle,(this->CamAngleX/angle),(this->CamAngleY/angle),0);
  glMatrixMode(GL_MODELVIEW_MATRIX);

}

void RenderEngine::generateColorPointer(int size,std:: vector<int> color_rgb, float* mat){
  for(int i=0;i<size;i++){
    mat[3*i] = (float)color_rgb[0]/255.0f;
    mat[3*i+1] = (float)color_rgb[1]/255.0f;
    mat[3*i+2] = (float)color_rgb[2]/255.0f;
  }
}

void RenderEngine::renderRoad() {
    // Render the road in gray
    float l = (float) this->targetRoad->length, w = (float) this->targetRoad->width, s = (float) this->targetRoad->signalPosition;
    float roadvertices[] =
    {  -(l)/2,-1.1,-(w)/2, -l/2,-1.1,w/2, l/2,-1.1,w/2, l/2,-1.1, -w/2
    };
    glVertexPointer(3, GL_FLOAT, 0, roadvertices);
    glColorPointer(3, GL_FLOAT, 0,NULL);
    glDrawArrays(GL_POLYGON, 0, 4);

    // Draw Lanes Markers
    float lanewidth = this->targetRoad->width/(float)this->targetRoad->lanes;
    float lanecolors[4*3];
    std::vector<int> c(3,255);
    this->generateColorPointer(4,c,lanecolors);
    for(int i=0;i<this->targetRoad->lanes-1;i++){
      float lanevertices[] =
      {  -(l)/2,-1.09,((i+1)*lanewidth)-(w)/2-0.05f, -l/2,-1.09,((i+1)*lanewidth)-(w)/2+0.05f, l/2,-1.09,((i+1)*lanewidth)-(w)/2+0.05f, l/2,-1.09, ((i+1)*lanewidth)-(w)/2-0.05f
      };
      glVertexPointer(3, GL_FLOAT, 0, lanevertices);
      glColorPointer(3, GL_FLOAT, 0, lanecolors);
      glDrawArrays(GL_POLYGON, 0, 4);
    }
    // Draw Signal
    float signalvertices[] =
    {  (s-(l)/2),1.1,-(w)/2, (s-(l)/2),1.1,w/2, (s-(l)/2),1.5,w/2, (s-(l)/2),1.5, -w/2,
       (float)(s-(l)/2)+0.5f,1.1,-(w)/2, (float)(s-(l)/2)+0.5f,1.1,w/2, (float)(s-(l)/2)+0.5f,1.5,w/2, (float)(s-(l)/2)+0.5f, 1.5, -w/2,
       (float)(s-(l)/2),1.1,-(w)/2, (float)(s-(l)/2),1.1,w/2, (float)(s-(l)/2)+0.5f,1.1,w/2, (float)(s-(l)/2)+0.5f, 1.1, -w/2,
       (float)(s-(l)/2),1.5,-(w)/2, (float)(s-(l)/2),1.5,w/2, (float)(s-(l)/2)+0.5f,1.5,w/2, (float)(s-(l)/2)+0.5f, 1.5, -w/2,
       (float)(s-(l)/2),1.1,(w)/2, (float)(s-(l)/2),1.5,w/2, (float)(s-(l)/2)+0.5f,1.5,w/2, (float)(s-(l)/2)+0.5f, 1.1, w/2,
       (float)(s-(l)/2),1.1,-(w)/2, (float)(s-(l)/2),1.5,-w/2, (float)(s-(l)/2)+0.5f,1.5,-w/2, (float)(s-(l)/2)+0.5f, 1.1, -w/2
    };

    glVertexPointer(3, GL_FLOAT, 0, signalvertices);
    float signalcolors[24*3];
    this->generateColorPointer(24,this->targetRoad->signal_rgb,signalcolors);
    glColorPointer(3, GL_FLOAT, 0, signalcolors);
    glDrawArrays(GL_POLYGON, 0, 24);

}

void RenderEngine::endSim() {
    glfwDestroyWindow(this->window);
    glfwTerminate();
}

void RenderEngine::initializeModels(){
    float vertices[] =
    {  -1/2.5, -1, -0.5,   -1/2.5,  1,-0.5,    -1/2.5,   1, 0.5,   -1/2.5, -1, 0.5, // back
       -1/2.5,  1,  0.5,     1/2.5,  1,  0.5,   1/2.5,0.8,0.5,      -1/2.5,0.8,0.5,
       -1/2.5,0.8,0.5,     1.5/2.5,0.8,0.5,    1.5/2.5,  -1, 0.5,   -1/2.5, -1, 0.5,
       -1/2.5,  1,  -0.5,     1/2.5,  1,  -0.5,   1/2.5,0.8,-0.5,      -1/2.5,0.8,-0.5,
       -1/2.5,0.8,- 0.5,     1.5/2.5,0.8,-0.5,    1.5/2.5,  -1, -0.5,   -1/2.5, -1, -0.5,
       1/2.5,0.8,0.5,      1 /2.5, 0.8, -0.5,     1/2.5, 1, -0.5,       1/2.5,1,0.5,
       1/2.5,0.8,0.5,     1.5/2.5, 0.8, 0.5,     1.5/2.5, 0.8, -0.5,   1/2.5, 0.8, -0.5,
        1/2.5, 1,  0.5,   1/2.5,  1,  -0.5,    -1/2.5,  1,  -0.5,     -1/2.5, 1,  0.5,  // top
        1.5/2.5, -1, 0.5, 1.5/2.5,-1, -0.5,   -1/2.5, -1, -0.5,    -1/2.5, -1, 0.5,
      1.5/2.5, 0.8, 0.5,    1.5/2.5, 0.8, -0.5,   1.5/2.5, -1, -0.5,    1.5/2.5, -1, 0.5
    };
    std::vector<float> v (vertices,vertices+sizeof(vertices)/sizeof(float));
    this->models.push_back(std::make_pair("truck",std::make_pair(v,v.size())));

    // CAR
    // float verticesc[] =
    // {
    //   -1.5/2, -1, -0.5,  -1.5/2, 0, -0.5,  -1.5/2, 0, 0.5,  -1.5/2, -1, 0.5,
    // -1.5/2, 0, 0.5,    -1.5/2, 0, -0.5,  -1/2, 0, -0.5,   -1/2, 0, 0.5,
    // -1.5/2, 0, 0.5,    -1.5/2, -1, 0.5,  -1/2, -1, 0.5,   -1/2, 0, 0.5,
    // -1.5/2, 0, -0.5,    -1.5/2, -1, -0.5,  -1/2, -1,- 0.5,   -1/2, 0, -0.5,
    // -1/2, 0, -0.5,   -1/2,  1,-0.5,    -1,   1/2, 0.5,   -1/2, 0, 0.5, // back
    //  -1/2,  1,  0.5,     1/2,  1,  0.5,   1/2,0,0.5,      -1/2,0,0.5,
    //  -1/2,0,0.5,     1.5/2,0,0.5,    1.5/2,  -1/2, 0.5,   -1/2, -1, 0.5,
    //  -1/2,  1,  -0.5,     1/2,  1,  -0.5,   1/2,0,-0.5,      -1/2,0,-0.5,
    //  -1/2,0,- 0.5,     1.5/2,0,-0.5,    1.5/2,  -1, -0.5,   -1/2, -1, -0.5,
    //  1/2,0,0.5,      1/2 , 0, -0.5,     1/2, 1, -0.5,       1/2,1,0.5,
    //  1/2,0,0.5,     1.5/2, 0, 0.5,     1.5/2, 0, -0.5,   1/2, 0, -0.5,
    //   1/2, 1,  0.5,   1/2,  1,  -0.5,    -1/2,  1,  -0.5,     -1/2, 1,  0.5,  // top
    //   1.5/2, -1, 0.5, 1.5/2,-1, -0.5,   -1.5/2, -1, -0.5,    -1.5/2, -1, 0.5,
    // 1.5/2, 0, 0.5,    1.5/2, 0, -0.5,   1.5/2, -1, -0.5,    1.5/2, -1, 0.5
    // };
    // std::vector<float> vd (verticesc,verticesc+sizeof(verticesc)/sizeof(float));
    // this->models.push_back(std::make_pair("car",std::make_pair(vd,vd.size())));
}

void RenderEngine::addModel(std::string type,float* vertices, int size){
  for(int i=0;i<this->models.size();i++){
    if(!this->models[i].first.compare(type)){
      std::vector<float> v(vertices,vertices+size);
      this->models[i].second.first = v;
      this->models[i].second.second = size;
      return;
    }
  }
  std::vector<float> v(vertices,vertices+size);
  this->models.push_back(std::make_pair(type,std::make_pair(v,size)));
}

void RenderEngine::renderVehicle(Vehicle* vehicle) {
    if(this->models.size()<1){
      std::cout << "[ ERROR ] - No Models for Rendering!"<<std::endl;
      std::exit(1);
    }
    if (vehicle->isOnRoad) {
      std::vector<float> tmp;
      int size = -1;
      for(auto model : this->models){
        if(!model.first.compare(vehicle->type)){
          tmp = model.second.first;
          size = model.second.second;
          break;
        }
      }
      if(size == -1){
        tmp = this->models.back().second.first;
        size = this->models.back().second.second;
      }
    float vertices[size];
    // std::cout <<"RENDERERERERER: "<< vehicle->currentPosition.first << std::endl;
    std::copy(tmp.begin(),tmp.end(),vertices);
    glPushMatrix();
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    float colors[size];
    this->generateColorPointer(size/3,vehicle->color_rgb,colors);
    glScalef(vehicle->length,1.0,vehicle->width);
    glTranslatef((float)(vehicle->currentPosition.first-(this->targetRoad->length/2) - (vehicle->length)/2)/(vehicle->length),0,(float)(-vehicle->currentPosition.second + (this->targetRoad->width/2) + vehicle->width/2)/(vehicle->width));
    glColorPointer(3, GL_FLOAT, 0, colors);
    if(vehicle->currentSpeed > 0){
      double theta = atan((vehicle->changeDirection*vehicle->verticalSpeed)/vehicle->currentSpeed) * 180 / M_PI; // in radians
      glRotatef(theta,0,1,0);
    }
    else{
      glRotatef(0,0,1,0);
    }
    glDrawArrays(GL_POLYGON, 0, size/3);

    glPopMatrix();


    }
}
