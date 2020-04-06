#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <csci441/shader.h>
#include <csci441/matrix4.h>
#include <csci441/matrix3.h>
#include <csci441/vector4.h>
#include <csci441/uniform.h>

#include "shape.h"
#include "model.h"
#include "camera.h"
#include "renderer.h"
#include "object.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;
char mazeFilename[] = "../models/TallMaze.obj";
char objectFilename[] = "../models/bunny.obj";
int mode = 0;
float cameraX = 0.2;
float cameraY = 0.1;
float cameraZ = 3;

float cameraBirdX = 0;
float cameraBirdY = 9;
float cameraBirdZ = 4;

Vector4 cameraBirdPos = Vector4(cameraBirdX, cameraBirdY, cameraBirdZ);
Vector4 cameraBirdFront = Vector4(0.0f, -8.0f, -0.6f);
Vector4 cameraBirdUp = Vector4(0.0f, 1.0f, 0.0f);



float currentTransX = 0;
float currentTransY = 0;
float currentTransZ = 4;

bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  SCREEN_WIDTH / 2.0;
float lastY =  SCREEN_HEIGHT / 2.0;

Vector4 cameraPos = Vector4(cameraX, cameraY, cameraZ);
Vector4 cameraFront = Vector4(0.0f, 0.0f, -1.0f);
Vector4 cameraUp = Vector4(0.0f, 1.0f, 0.0f);

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

bool isPressed(GLFWwindow *window, int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

bool isReleased(GLFWwindow *window, int key) {
    return glfwGetKey(window, key) == GLFW_RELEASE;
}

Matrix4 processModel(const Matrix4& model, GLFWwindow *window) {
    Matrix4 trans;
    Matrix4 forwardTrans;
    Matrix4 backwardTrans;

    const float ROT = 1;
    const float SCALE = .05;
    const float TRANS = .01;
    const float TRANS1 = .05;
    const float CAMINC = .03;



    // ROTATE
    if (isPressed(window, GLFW_KEY_U)) {
      backwardTrans.translate(-currentTransX, -currentTransY, -currentTransZ);
      trans.rotate_x(-ROT);
      forwardTrans.translate(currentTransX, currentTransY, currentTransZ);
    }
    else if (isPressed(window, GLFW_KEY_I)) {
      backwardTrans.translate(-currentTransX, -currentTransY, -currentTransZ);
      trans.rotate_x(ROT);
      forwardTrans.translate(currentTransX, currentTransY, currentTransZ);
    }
    else if (isPressed(window, GLFW_KEY_O)) {
      backwardTrans.translate(-currentTransX, -currentTransY, -currentTransZ);
      trans.rotate_y(-ROT);
      forwardTrans.translate(currentTransX, currentTransY, currentTransZ);
    }
    else if (isPressed(window, GLFW_KEY_P)) {
      backwardTrans.translate(-currentTransX, -currentTransY, -currentTransZ);
      trans.rotate_y(ROT);
      forwardTrans.translate(currentTransX, currentTransY, currentTransZ);
    }
    else if (isPressed(window, '[')) {
      backwardTrans.translate(-currentTransX, -currentTransY, -currentTransZ);
      trans.rotate_z(-ROT);
      forwardTrans.translate(currentTransX, currentTransY, currentTransZ);
    }
    else if (isPressed(window, ']')) {
      backwardTrans.translate(-currentTransX, -currentTransY, -currentTransZ);
      trans.rotate_z(ROT);
      forwardTrans.translate(currentTransX, currentTransY, currentTransZ);
    }
    // SCALE
    else if (isPressed(window, '-')) {
      trans.scale(1-SCALE, 1-SCALE, 1-SCALE); }
    else if (isPressed(window, '=')) { trans.scale(1+SCALE, 1+SCALE, 1+SCALE); }
    // TRANSLATE
    else if (isPressed(window, ',')) {
      trans.translate(0, TRANS, 0);
      cameraY += TRANS;
      currentTransY += TRANS;
      std::cout << currentTransY << '\n';
    }
    else if (isPressed(window, '.')) {
      trans.translate(0, -TRANS, 0);
      cameraY -= TRANS;
      currentTransY -= TRANS;
    }
    else if (isPressed(window, GLFW_KEY_LEFT)) {
      trans.translate(-TRANS, 0, 0);
      cameraX -= TRANS;
      currentTransX -= TRANS;
    }
    else if (isPressed(window, GLFW_KEY_RIGHT)) {
      trans.translate(TRANS, 0, 0);
      cameraX += TRANS;
      currentTransX += TRANS;
    }
    else if (isPressed(window, GLFW_KEY_DOWN)) {
      trans.translate(0,0,TRANS);
      cameraZ += TRANS;
      currentTransZ += TRANS;
    }
    else if (isPressed(window, GLFW_KEY_UP)) { trans.translate(0,0,-TRANS);
      cameraZ -= TRANS;
      currentTransZ -= TRANS;
    }
    //Camera Panning
    else if (isPressed(window, GLFW_KEY_W)) {
      if (cameraBirdZ > -6.8){
        cameraBirdZ -= CAMINC;
        std::cout << cameraBirdZ << '\n';
      }
    }
    else if (isPressed(window, GLFW_KEY_A)) {
      if (cameraBirdX > -9){
        cameraBirdX -= CAMINC;
        std::cout << cameraBirdX << '\n';
      }

    }
    else if (isPressed(window, GLFW_KEY_S)) {
      if (cameraBirdZ < 4){
        cameraBirdZ += CAMINC;
        std::cout << cameraBirdZ << '\n';
      }

    }
    else if (isPressed(window, GLFW_KEY_D)) {
      if (cameraBirdX < 2){
        cameraBirdX += CAMINC;
        std::cout << cameraBirdX << '\n';
      }

    }


    return forwardTrans * trans * backwardTrans * model;
}

void processInput(Matrix4& model, GLFWwindow *window) {
    if (isPressed(window, GLFW_KEY_ESCAPE) || isPressed(window, GLFW_KEY_Q)) {
        glfwSetWindowShouldClose(window, true);
    }
    else if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        mode++;
        glfwWaitEventsTimeout(0.7);
    }
    model = processModel(model, window);
}

void errorCallback(int error, const char* description) {
    fprintf(stderr, "GLFW Error: %s\n", description);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.05f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    Vector4 front = Vector4(cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch));
    cameraFront = front.normalized();
}


int main(void) {
    GLFWwindow* window;

    glfwSetErrorCallback(errorCallback);

    /* Initialize the library */
    if (!glfwInit()) { return -1; }

    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CSCI441-lab", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // tell glfw what to do on resize
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // init glad
    if (!gladLoadGL()) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        glfwTerminate();
        return -1;
    }


    // create obj
    Model bunny(
            Object(objectFilename, 1.0, 0.0, 0.0).coordsSmooth,
            Shader("../vert.glsl", "../frag.glsl"));

    Model maze(
            Object(mazeFilename, 1.0, 1.0, 1.0).coordsFlat,
            Shader("../vert.glsl", "../frag.glsl"));

    // make a floor
    Model floor(
            DiscoCube().coords,
            Shader("../vert.glsl", "../frag.glsl"));



    Matrix4 floor_trans, floor_scale, maze_scale, maze_trans, maze_rotX,maze_rotY, bunny_scale, bunny_trans, bunny_rotY;



    bunny_scale.scale(.8, .8, .8);
    bunny_trans.translate(0, 0, 4);
    maze_scale.scale(.02, .02, .02);
    maze_rotX.rotate_x(-90);
    maze_rotY.rotate_y(180);
    bunny_rotY.rotate_y(-90);
    maze_trans.translate(2, -1, -7);
    floor_trans.translate(0, -2, 0);
    floor_scale.scale(100, 1, 100);

    bunny.model = bunny_trans * bunny_scale;
    maze.model =  maze_trans * maze_rotY * maze_rotX * maze_scale;
    floor.model = floor_trans*floor_scale;

    // setup camera
    Matrix4 projection;
    projection.perspective(45, 1, .01, 10);


    Camera camera;
    camera.projection = projection;
    camera.eye = cameraPos;
    camera.gaze = cameraFront + cameraPos;
    camera.up = cameraUp;

    // and use z-buffering
    glEnable(GL_DEPTH_TEST);

    // create a renderer
    Renderer renderer;

    // set the light position
    Vector4 lightPos(3.75f, 3.75f, 4.0f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        // process input
        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render the object and the floor
        if (mode % 2 == 0){
          cameraPos = Vector4(cameraX, cameraY, cameraZ);
          camera.eye = cameraPos;
          camera.gaze = cameraPos + cameraFront;
        }
        else{
          cameraBirdPos = Vector4(cameraBirdX, cameraBirdY, cameraBirdZ);
          camera.eye = cameraBirdPos;
          camera.gaze = cameraBirdPos + cameraBirdFront;
        }

        processInput(bunny.model, window);
        renderer.render(camera, bunny, lightPos);
        renderer.render(camera, maze, lightPos);


        /* Swap front and back and poll for io events */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
