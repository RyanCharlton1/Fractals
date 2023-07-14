#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include <fstream>
#include <sstream>

//#define DEBUG

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

    GLFWwindow *window = glfwCreateWindow(800, 600, "Mandelbrot", NULL, NULL);
    if(!window){
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, keycallback);

    int vert, frag;
    int prog;

    // Create opengl shader object(ints are references)
    vert = glCreateShader(GL_VERTEX_SHADER);
    frag = glCreateShader(GL_FRAGMENT_SHADER);

    // Load shader code
    std::ifstream vertf("shaders/m.vert");
    std::stringstream vertsb;
    vertsb << vertf.rdbuf();
    std::string verts = vertsb.str();

    std::ifstream fragf("shaders/m.frag");
    std::stringstream fragsb;
    fragsb << fragf.rdbuf();
    std::string frags = fragsb.str();

#ifdef DEBUG
    // Print shader code for debug
    std::cout << "Vetex shader:" << std::endl;
    std::cout << vertsb.str() << std::endl << std::endl;
    std::cout << "Fragment shader:" << std::endl;
    std::cout << fragsb.str() << std::endl << std::endl;
#endif

    // Associate shader code with shader objects
    const char *ptr = verts.c_str();
    glShaderSource(vert, 1, &ptr, NULL);
    ptr = frags.c_str();
    glShaderSource(frag, 1, &ptr, NULL);

    // Compile shader code
    glCompileShader(vert);
    glCompileShader(frag);

    // Check compile success
    int vertcomp = 0, fragcomp = 0;
    glGetShaderiv(vert, GL_COMPILE_STATUS, &vertcomp);
    glGetShaderiv(frag, GL_COMPILE_STATUS, &fragcomp);

    int loglen;
    // Print error log if vert wasn't successfully compiled 
    if(vertcomp == 0){
        // Get info log length
        glGetShaderiv(vert, GL_INFO_LOG_LENGTH, &loglen);
        // Create char array to store log
        char *infolog = new char[loglen + 1];
        infolog[loglen] = '\0'; // null terminate
        // Store log in infolog
        glGetShaderInfoLog(vert, loglen, &loglen, infolog);
        std::cout << infolog << std::endl;
        delete infolog;
        // Prevent render loop begining 
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        // Clear up shader
        glDeleteShader(vert);
    }
    // Repeat for frag
    if(fragcomp == 0){
        glGetShaderiv(frag, GL_INFO_LOG_LENGTH, &loglen);
        char *infolog = new char[loglen + 1];
        infolog[loglen] = '\0';
        glGetShaderInfoLog(frag, loglen, &loglen, infolog);
        std::cout << infolog << std::endl;
        delete infolog;
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        glDeleteShader(vert);
        glDeleteShader(frag);
    }

    // Create opengl program object(int is reference)
    prog = glCreateProgram();    

    // Attach (compiled)shaders to program
    glAttachShader(prog, vert);
    glAttachShader(prog, frag);
    
    // Link shaders together to for program
    glLinkProgram(prog);

    // Check link success
    int proglink = 0;
    glGetProgramiv(prog, GL_LINK_STATUS, &proglink);

    // Same as proces as shaders
    if(proglink == 0){
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &loglen);
        char *infolog = new char[loglen + 1];
        infolog[loglen] = '\0';
        glGetProgramInfoLog(prog, loglen, &loglen, infolog);
        std::cout << infolog << std::endl;
        delete infolog;
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        // Clean up shaders and program
        glDeleteShader(vert);
        glDeleteShader(frag);
        glDeleteProgram(prog);
    }

    while(!glfwWindowShouldClose(window)){
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}