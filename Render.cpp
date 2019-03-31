#include <bits/stdc++.h>
#include "Vehicle.h"
#include "Road.h"
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include "Render.h"

RenderEngine::RenderEngine(Road* targetRoad) {
    std::cout << "Instantiated RenderEngine for road " << targetRoad->id << std::endl;
    this->targetRoad = targetRoad;
    // Dividing by this factor gives it in viewport dimensions
    // this->scalex = 25;
    // this->scaley = 50;
    this->signalSize = 1;
    // Set the default background color
    this->bgcolor.push_back(0.701f);
    this->bgcolor.push_back(0.341f);
    this->bgcolor.push_back(0.1f);
    this->monitorWidth = 1024;
    this->monitorHeight = 620;

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
    glfwSetKeyCallback(window, this->key_callback);

    // Get info of GPU and supported OpenGL version
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST); // Depth Testing
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    this->window = window;
}

float RenderEngine::getTime() {
    float time = glfwGetTime();
    return time;
}

void RenderEngine::render(double delT) {
    double beginTime =
    #ifdef RENDER_ENGINE_H
    RenderEngine::getTime();
    #else
    0;
    #endif
    double oldTime;
    double currentTime =
    #ifdef RENDER_ENGINE_H
    RenderEngine::getTime();
    #else
    0;
    #endif
    std::cout << "Starting Render routine"<< std::endl;
    while((currentTime - beginTime < delT) && !glfwWindowShouldClose(RenderEngine::window)) {
        // std::cout << "Rendering now..." << std::endl;
        this->targetRoad->updateSim(currentTime - beginTime);
        this->UpdateCamera(currentTime - beginTime );


        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        RenderEngine::renderRoad();

        // Render a rectangle moving at constant speed
        // glColor3f(0.0f, 0.0f, 0.0f);
        // float t = glfwGetTime();
        // glRectd(t/10.0-1, 0.8, t/10.0-0.98, 0.75);

        // std::cout << "num vehicles; " << this->targetRoad->vehicles.size() << std::endl;
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

        oldTime = currentTime;
        currentTime =
        #ifdef RENDER_ENGINE_H
        RenderEngine::getTime();
        #else
        #endif
    }
}

void RenderEngine::setCameraSpeed(float translationspeed, float rotationspeed, float zoomspeed){
  this->CamTranslationSpeed = translationspeed;
  this->CamRotationSpeed = rotationspeed;
  this->CamZoomSpeed = zoomspeed;
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
    {  -(l)/2,-1.1,-(w)/2, -l/2,-1.1,w/2, l/2,-1.1,w/2, l/2,-1.1, -w/2,
    };
    glVertexPointer(3, GL_FLOAT, 0, roadvertices);
    glColorPointer(3, GL_FLOAT, 0,NULL);
    glDrawArrays(GL_POLYGON, 0, 4);
    // Draw Signal
    float signalvertices[] =
    {  (s-(l)/2),1.1,-(w)/2, (s-(l)/2),1.1,w/2, (s-(l)/2),1.5,w/2, (s-(l)/2),1.5, -w/2,
       (float)(s-(l)/2)+0.5,1.1,-(w)/2, (float)(s-(l)/2)+0.5,1.1,w/2, (float)(s-(l)/2)+0.5,1.5,w/2, (float)(s-(l)/2)+0.5, 1.5, -w/2,
       (float)(s-(l)/2),1.1,-(w)/2, (float)(s-(l)/2),1.1,w/2, (float)(s-(l)/2)+0.5,1.1,w/2, (float)(s-(l)/2)+0.5, 1.1, -w/2,
       (float)(s-(l)/2),1.5,-(w)/2, (float)(s-(l)/2),1.5,w/2, (float)(s-(l)/2)+0.5,1.5,w/2, (float)(s-(l)/2)+0.5, 1.5, -w/2,
       (float)(s-(l)/2),1.1,(w)/2, (float)(s-(l)/2),1.5,w/2, (float)(s-(l)/2)+0.5,1.5,w/2, (float)(s-(l)/2)+0.5, 1.1, w/2,
       (float)(s-(l)/2),1.1,-(w)/2, (float)(s-(l)/2),1.5,-w/2, (float)(s-(l)/2)+0.5,1.5,-w/2, (float)(s-(l)/2)+0.5, 1.1, -w/2
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
    // std::cout << typeid(vertices)<<std::endl;
    this->models.push_back(std::make_pair("truck",std::make_pair(v,v.size())));
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
    std::copy(tmp.begin(),tmp.end(),vertices);
    glPushMatrix();
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    float colors[size];
    this->generateColorPointer(size/3,vehicle->color_rgb,colors);
    glScalef(vehicle->length,1.0,vehicle->width);
    glTranslatef((float)(vehicle->currentPosition.first-(this->targetRoad->length/2) - (vehicle->length)/2)/(vehicle->length),0,(float)(-vehicle->currentPosition.second + (this->targetRoad->width/2) + vehicle->width/2)/(vehicle->width));
    glColorPointer(3, GL_FLOAT, 0, colors);
    glDrawArrays(GL_POLYGON, 0, size/3);

    glPopMatrix();


    }
}
