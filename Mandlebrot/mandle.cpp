#include "GLFW/glfw3.h"
#include <iostream>

bool QUIT = false;

void keycallback(GLFWwindow *win, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ESCAPE)
            QUIT = true;
}

int main(){
    glfwInit();
    GLFWwindow *window = glfwCreateWindow(800, 600, "Mandlebrot", NULL, NULL);
    glfwSetKeyCallback(window, keycallback);

    while(!QUIT){
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}