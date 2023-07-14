#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>

// Square mesh over the screen
const float screen[] = {
    -1.0f, -1.0f,
    -1.0f, 1.0f,
     1.0f, 1.0f,
     
     1.0f, 1.0f,
     1.0f, -1.0f,
    -1.0f, -1.0f};

void keycallback(GLFWwindow *win, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(win, GLFW_TRUE);
}

int main(){
    if(!glfwInit()){
        std::cout << "GLFW failed to initialize" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Mandlebrot", NULL, NULL);
    if(!window){
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, keycallback);



    while(!glfwWindowShouldClose(window)){
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}