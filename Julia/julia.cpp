#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <ctime>

//#define DEBUG
#define WIDTH 800
#define HEIGHT 800

float targetx = -2.0, targety = -2.0;
float rangex = 4.0, rangey = 4.0;

// Square mesh over the screen
const float screen[] = {
    -1.0f, -1.0f,
    -1.0f, 1.0f,
     1.0f, 1.0f,
     
     1.0f, 1.0f,
     1.0f, -1.0f,
    -1.0f, -1.0f
};

bool PAUSE = false;
void keycallback(GLFWwindow *win, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(win, GLFW_TRUE);

    if (key == GLFW_KEY_R && action == GLFW_PRESS){
        targetx = -2.0;
        targety = -2.0;
        rangex = 4.0;
        rangey = 4.0;
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        PAUSE = !PAUSE;
}

void resizecallback(GLFWwindow *win, int width, int height){
    glViewport(0, 0, width, height);
}

int MOUSE = GLFW_RELEASE;
double lastx, lasty;
void clickcallback(GLFWwindow *win, int button, int action, int mods){
    // Mouse will be se to GLFW_PRESS or GLFW_RELEASE and curson position is noted
    if (button == GLFW_MOUSE_BUTTON_LEFT){
        MOUSE = action;
        glfwGetCursorPos(win, &lastx, &lasty);
    }
}

void input(GLFWwindow *win){
    if (MOUSE == GLFW_PRESS){
        // Get current curson position
        double currentx, currenty;
        glfwGetCursorPos(win, &currentx, &currenty);

        // Find diffence in cursor between frames
        double diffx = currentx - lastx;
        double diffy = currenty - lasty;

        // get window dimensions for scaling
        int w, h;
        glfwGetWindowSize(win, &w, &h);

        // Shift target by the (scaled) distance moved 
        targetx -= diffx / (double)w * rangex;
        targety += diffy / (double)h * rangey;

        // Set last for next time
        lastx = currentx;
        lasty = currenty;
    }
}

#define ZOOMSENSITIVIY 9.0
void scrollcallback(GLFWwindow *win, double xscroll, double yscroll){
    // Get cursor position
    double currentx, currenty;
    glfwGetCursorPos(win, &currentx, &currenty);
    
    // Get window dimensions
    int w, h;
    glfwGetWindowSize(win, &w, &h);
    // Correct for glfw having down as the positive y direction
    currenty = h - currenty;

    if (yscroll > 0.0){
        // Move the target position in around the cursor 
        targetx += currentx / (double)w * rangex / ZOOMSENSITIVIY;
        targety += currenty / (double)h * rangey / ZOOMSENSITIVIY;

        // Reduce the ranges to zoom
        rangex *= (ZOOMSENSITIVIY - 1.0) / ZOOMSENSITIVIY;
        rangey *= (ZOOMSENSITIVIY - 1.0) / ZOOMSENSITIVIY;
    } else if (yscroll < 0.0){
        // Move the target position out around the cursor
        targetx -= currentx / (double)w * rangex / (ZOOMSENSITIVIY - 1.0);
        targety -= currenty / (double)h * rangey / (ZOOMSENSITIVIY - 1.0);

        // Increase ranges to zoom out
        rangex *= ZOOMSENSITIVIY / (ZOOMSENSITIVIY - 1.0); 
        rangey *= ZOOMSENSITIVIY / (ZOOMSENSITIVIY - 1.0); 
    }
}

int main(){
    if(!glfwInit()){
        std::cout << "GLFW failed to initialize" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Julia", NULL, NULL);
    if(!window){
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "GLAD failed" << std::endl;
        return -1;
    }
    glfwSwapInterval(1);
    
    int frameBufferWidth, frameBufferHeight;
    glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
    glViewport(0, 0, frameBufferWidth, frameBufferHeight);
    glfwSetKeyCallback(window, keycallback);
    glfwSetWindowSizeCallback(window, resizecallback);
    glfwSetMouseButtonCallback(window, clickcallback);
    glfwSetScrollCallback(window, scrollcallback);

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

    unsigned int vbo;
    // Create buffer to put our square mesh
    glGenBuffers(1, &vbo);
    // Tell opengl which array buffer we're using 
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Send mesh data to bound array buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(screen), screen, GL_STATIC_DRAW);

    // Bind buffer to object that describes buffer
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    // Explain the data of the buffer as each element being 2 floats that are noramlized
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, 0, 0);
    glEnableVertexAttribArray(0);  
    
    float creal;
    float cimag;
    float angle = 0.0;
    float mag = 0.7885;

    double ftime = 0, totaltime = 0;
    clock_t fstart, fend;
    fstart = clock();
    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);

        // Tell opengl what shaders to use
        glUseProgram(prog);
        // Send canvas info to shaders
        glUniform1f(glGetUniformLocation(prog, "targetx"), targetx);
        glUniform1f(glGetUniformLocation(prog, "targety"), targety);
        glUniform1f(glGetUniformLocation(prog, "rangex"), rangex);
        glUniform1f(glGetUniformLocation(prog, "rangey"), rangey);
        // Calculate julia constant
        if(!PAUSE)
            angle += ftime * 2 * M_PI;
        creal = mag * cos(angle);
        cimag = mag * sin(angle);
        // Send constant to shaders
        glUniform1f(glGetUniformLocation(prog, "creal"), creal);
        glUniform1f(glGetUniformLocation(prog, "cimag"), cimag);

        // Tell opengl what mesh to use
        glBindVertexArray(vao);
        // Render current mesh with current shader program
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Input for movement of canvas
        input(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        fend = clock();
        ftime = (double)(fend - fstart) / (double)CLOCKS_PER_SEC;
        totaltime += ftime;
        fstart = fend;
    }

    glfwTerminate();
    return 0;
}