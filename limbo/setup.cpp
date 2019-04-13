#include <iostream>

// ALWAYS include GLEW before gl.h and glfw3.h
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;

void setupGlew() {
    glewExperimental = true; // Needed for core profile
    if( !glfwInit() ) {
        cout << "Failed to initialize GLFW" << endl;
        assert(0);
    }
}

GLFWwindow* getGLFWWindow() {
    GLFWwindow* window = glfwCreateWindow( 1024, 768, "Tutorial 01", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials." << endl;
        glfwTerminate();
        assert(0);
    }

    glfwMakeContextCurrent(window); // Initialize GLEW
    glewExperimental=true; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        cout << "Failed to initialize GLEW" << endl;
        assert(0);
    }

    return window;
}

void setupGLFW() {
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
}
