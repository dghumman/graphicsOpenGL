#include <limbo/setup.hpp>

class MouseTracker {
    GLFWwindow* mWindow;
    double mLastX, mLastY;
public:
    MouseTracker(GLFWwindow* window) {
        mWindow = window;
        glfwGetCursorPos(mWindow, &mLastX, &mLastY);
    }
    
    void get_motion_since_last_check(double * x, double * y) {
        glfwGetCursorPos(mWindow, x, y);
        *x -= mLastX;
        *y -= mLastY;
        mLastX += *x;
        mLastY += *y;
        
        // Probably add some edge of screen bounds
    }
};
