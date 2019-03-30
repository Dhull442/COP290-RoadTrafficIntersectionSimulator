#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <bits/stdc++.h>
#include <cstdio>

void controls(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS)
        if(key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, GL_TRUE);
}

GLFWwindow* initWindow(const int resX, const int resY)
{
    if(!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return NULL;
    }
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing

    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow(resX, resY, "TEST", NULL, NULL);

    if(window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, controls);

    // Get info of GPU and supported OpenGL version
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST); // Depth Testing
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    return window;
}
void generateColorPointer(int size,std:: vector<float> color_rgb, GLfloat* mat){
  for(int i=0;i<size;i++){
    mat[3*i] = color_rgb[0];
    mat[3*i+1] = color_rgb[1];
    mat[3*i+2] = color_rgb[2];
  }
}
void drawCube(int x)
{
    GLfloat vertices[] =
    {  -1, -1, -0.5,   -1,  1,-0.5,    -1,   1, 0.5,   -1, -1, 0.5, // back
       -1,  1,  0.5,     1,  1,  0.5,   1,0.8,0.5,      -1,0.8,0.5,
       -1,0.8,0.5,     1.5,0.8,0.5,    1.5,  -1, 0.5,   -1, -1, 0.5,
       -1,  1,  -0.5,     1,  1,  -0.5,   1,0.8,-0.5,      -1,0.8,-0.5,
       -1,0.8,- 0.5,     1.5,0.8,-0.5,    1.5,  -1, -0.5,   -1, -1, -0.5,
       1,0.8,0.5,      1 , 0.8, -0.5,     1, 1, -0.5,       1,1,0.5,
       1,0.8,0.5,     1.5, 0.8, 0.5,     1.5, 0.8, -0.5,   1, 0.8, -0.5,
        1, 1,  0.5,   1,  1,  -0.5,    -1,  1,  -0.5,     -1, 1,  0.5,  // top
        1.5, -1, 0.5, 1.5,-1, -0.5,   -1, -1, -0.5,    -1, -1, 0.5,
      1.5, 0.8, 0.5,    1.5, 0.8, -0.5,   1.5, -1, -0.5,    1.5, -1, 0.5
    };
    std::vector<float> c(3,0); c[0]= 0.5, c[1]=0.4, c[2] = 0.4;
    GLfloat colors[40*3];
    generateColorPointer(40,c,colors);
    static float alpha = 0;
    //attempt to rotate cube
    /* We have a color array and a vertex array */
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    // glTranslatef(x/(float)20, 0, -1);

    glColorPointer(3, GL_FLOAT, 0, colors);

    glDrawArrays(GL_POLYGON, 0, 40);

    /* Cleanup states */
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    alpha += 1;
}
void display( GLFWwindow* window )
{   float x = 0, newx=0, newy=0,angleX=0,angleY=0,zoom = -10 ;
    while(!glfwWindowShouldClose(window))
    {
        // Scale to window size
        GLint windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);
        // Draw stuff
        glClearColor(0.0, 0.8, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glutMouseFunc(mouse_click);
        // glfwGetCursorPos(window,&newx, &newy);
        // double horizontalAngle, verticalAngle,mouseSpeed = 1, deltaTime = 0.05;
        // horizontalAngle += mouseSpeed * deltaTime * float(1024/2 - xpos );
        // verticalAngle   += mouseSpeed * deltaTime * float( 768/2 - ypos );
        glMatrixMode(GL_PROJECTION_MATRIX);
        glLoadIdentity();
        gluPerspective( 90, (double)windowWidth / (double)windowHeight, 0.1, 100 );
        // double delX, delY;
//         glm::vec3 direction(
//     cos(verticalAngle) * sin(horizontalAngle),
//     sin(verticalAngle),
//     cos(verticalAngle) * cos(horizontalAngle)
// );
        if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
        newy -= 0.1;
        }
        // Move backward
        if (glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS){
          newy += 0.1;
          }
        // Strafe right
        if (glfwGetKey(window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
          newx -= 0.1;
        }
        // Strafe left
        if (glfwGetKey(window, GLFW_KEY_LEFT ) == GLFW_PRESS){
        newx += 0.1;
        }
        if (glfwGetKey(window, GLFW_KEY_W ) == GLFW_PRESS){
        angleX += 0.5;
        // glRotatef(angleX,1,0,0);
        }
        if (glfwGetKey(window, GLFW_KEY_S ) == GLFW_PRESS){
        angleX -= 0.5;
        // glRotatef(angleX,1,0,0);
        }
        if (glfwGetKey(window, GLFW_KEY_A ) == GLFW_PRESS){
        angleY += 0.5;
        // glRotatef(angleY,0,1,0);
        }
        if (glfwGetKey(window, GLFW_KEY_D ) == GLFW_PRESS){
        angleY -= 0.5;
        }
        if (glfwGetKey(window, GLFW_KEY_Z ) == GLFW_PRESS){
        zoom += 1;
        }
        if (glfwGetKey(window, GLFW_KEY_X ) == GLFW_PRESS){
        zoom -= 1;
        }

        if (glfwGetKey(window, GLFW_KEY_R ) == GLFW_PRESS){
        zoom = -10;
        angleX = 0;
        angleY = 0;
        newx = 0;
        newy = 0;
        }
        double angle = sqrt(pow(angleX,2)+pow(angleY,2));
        glTranslatef(newx,newy,zoom);
        glRotatef(angle,(angleX/angle),(angleY/angle),0);
        glMatrixMode(GL_MODELVIEW_MATRIX);

        drawCube(x);

        // Update Screen
        glfwSwapBuffers(window);

        // Check for any input, or window movement
        glfwPollEvents();
        x+=0.05;
    }
}

int main(int argc, char** argv)
{
    GLFWwindow* window = initWindow(1024, 620);
    if( NULL != window )
    {
        display( window );
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
