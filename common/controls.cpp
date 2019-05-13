#include <limbo/setup.hpp>

// position
glm::vec3 position = glm::vec3( 0, 0, 5 );
// horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// vertical angle : 0, look at the horizon
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;

void computeMatricesFromInputs(GLFWwindow* window){
    
    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();
    
    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    // Get mouse position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    // Reset mouse position for next frame
    glfwSetCursorPos(window, 1024/2, 768/2);

    // Compute new orientation
    horizontalAngle += mouseSpeed * deltaTime * float(1024/2 - xpos );
    verticalAngle   += mouseSpeed * deltaTime * float( 768/2 - ypos );


    // Direction : Spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction(
                        cos(verticalAngle) * sin(horizontalAngle),
                        sin(verticalAngle),
                        cos(verticalAngle) * cos(horizontalAngle)
                        );

    // Right vector
    glm::vec3 right = glm::vec3(
                                sin(horizontalAngle - 3.14f/2.0f),
                                0,
                                cos(horizontalAngle - 3.14f/2.0f)
                                );

    // Up vector : perpendicular to both direction and right
//    glm::vec3 up = glm::cross( right, direction );


    // Move forward
    if (glfwGetKey(window, GLFW_KEY_UP ) == GLFW_PRESS){
        position += direction * deltaTime * speed;
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS){
        position -= direction * deltaTime * speed;
    }
    // Strafe right
    if (glfwGetKey(window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
        position += right * deltaTime * speed;
    }
    // Strafe left
    if (glfwGetKey(window, GLFW_KEY_LEFT ) == GLFW_PRESS){
        position -= right * deltaTime * speed;
    }

//    double currentTime = glfwGetTime();
//    float deltaTime = float(currentTime - lastTime);

//    float FoV = initialFoV - 5 * glfwGetMouseWheel();
//    float FoV = initialFoV;
    
//    // Projection matrix : 45&deg; Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
//    const glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
//    // Camera matrix
//    const glm::mat4 ViewMatrix       = glm::lookAt(
//                                   position,           // Camera is here
//                                   position+direction, // and looks here : at the same position, plus "direction"
//                                   up                  // Head is up (set to 0,-1,0 to look upside-down)
//                                   );
    
    // For the next frame, the "last time" will be "now"
    lastTime = currentTime;
}
