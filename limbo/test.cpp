// Include standard headers
#include <stdio.h>
#include <stdlib.h>

#include <limbo/setup.hpp>

#include <camera/camera_manager.hpp>

#include <common/controls.hpp>
#include <common/shader.hpp>
#include <common/buildShapes.hpp>

float float_rand( float min, float max ) {
    float scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
    return min + scale * ( max - min );      /* [min, max] */
}

int main(){
    setupGlew(); // Must be before setupGLFW
    setupGLFW();
    GLFWwindow* window = getGLFWWindow();
 
    // The tutorial said to not think about these 3 lines very much
    {
        GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);
    }
    
    MouseTracker mouseTracker(window);
    CameraManager cameraManager;
    
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );
    
    // Get a handle for our "MVP" uniform
    // Only during the initialisation
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");


    // I've just created a window, now gonna make a Vertex Array Object for the triangle
    // An array of 12 vectors which represents 3 vertices
    static vector<Triangle> unsortedTriangles, sortedTriangles;
    ShapeBuilder::buildCube(0,0,0,1,&unsortedTriangles);
    vector<pair<float, int> > triangleSortData;
    static vector<GLfloat> g_vertex_buffer;

    sortedTriangles.resize(unsortedTriangles.size());
    triangleSortData.resize(unsortedTriangles.size());


    // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 ) {

    
    
    for (int i=0; i<unsortedTriangles.size(); i++) {
        glm::vec3 camPos  = cameraManager.getCameraPosition();
        float dist = -unsortedTriangles[i].fakedist2(Point(camPos.x, camPos.y, camPos.z));
        triangleSortData[i] = (make_pair(dist, i));
    }
    std::sort(triangleSortData.begin(), triangleSortData.end());
    for (int i=0; i<triangleSortData.size(); i++) {
        sortedTriangles[i] = unsortedTriangles[triangleSortData[i].second];
    }
    g_vertex_buffer.resize(0);
    ShapeBuilder::convertToPoints(sortedTriangles, &g_vertex_buffer);
    
    // This will identify our vertex buffer
    GLuint vertexbuffer;
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer.size()*sizeof(g_vertex_buffer.data()), g_vertex_buffer.data(), GL_STATIC_DRAW);
    
    // One color for each vertex. They were generated randomly.
    static vector<GLfloat> g_color_buffer;
    for (int i=0; i<g_vertex_buffer.size(); i++) {
        g_color_buffer.push_back(float_rand( 0.0f, 1.0f));
    }
    
    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, g_color_buffer.size()*sizeof(g_color_buffer.data()), g_color_buffer.data(), GL_STATIC_DRAW);
    
    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glVertexAttribPointer(
      1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
      3,                                // size
      GL_FLOAT,                         // type
      GL_FALSE,                         // normalized?
      0,                                // stride
      (void*)0                          // array buffer offset
    );

    
//    // Check if the ESC key was pressed or the window was closed
//    while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
//           glfwWindowShouldClose(window) == 0 ) {

        glm::mat4 mvp;
        {
            double x,y;
            mouseTracker.get_motion_since_last_check(&x, &y);
            cameraManager.change_camera_perspective(x/100, y/100);
            const glm::mat4 Projection = cameraManager.getProjectionMatrix();
            const glm::mat4 View = cameraManager.getViewMatrix();
            const glm::mat4 Model = glm::mat4(1.0f);
            
            // Our ModelViewProjection : multiplication of our 3 matrices
            mvp = Projection * View * Model;
            
        }
        
        // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

        // Make the triangle after clearing the screen, right???
        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                              0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                              3,                  // size
                              GL_FLOAT,           // type
                              GL_FALSE,           // normalized?
                              0,                  // stride
                              (void*)0            // array buffer offset
                              );


        glUseProgram(programID);

        
        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, g_vertex_buffer.size()/3); // Starting from vertex 0; 3 vertices total -> 1 triangle
        glDisableVertexAttribArray(0);
        
        

        
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
