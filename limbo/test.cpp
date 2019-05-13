// Include standard headers
#include <stdio.h>
#include <stdlib.h>

#include <limbo/setup.hpp>

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
 
    glm::mat4 mvp;
    {
        // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        const glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.f/3, 0.1f, 100.0f);
        
        // Setup a camera, look at the origin
        const glm::vec3 CameraPosition(4,3,3);
        const glm::vec3 Origin(0,0,0);
        const glm::vec3 UpDirection(0,1,0);
        glm::mat4 View = glm::lookAt(CameraPosition, Origin, UpDirection);
        
        // Model matrix : an identity matrix (model will be at the origin)
        glm::mat4 Model = glm::mat4(1.0f);
        
        // Our ModelViewProjection : multiplication of our 3 matrices
        mvp = Projection * View * Model;
    }

    // The tutorial said to not think about these 3 lines very much
    {
        GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);
    }
    
    // I've just created a window, now gonna make a Vertex Array Object for the triangle
    // An array of 12 vectors which represents 3 vertices
    static vector<GLfloat> g_vertex_buffer;
    ShapeBuilder::buildCube(0,0,0,1,&g_vertex_buffer);

    
    // This will identify our vertex buffer
    GLuint vertexbuffer;
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer.size()*sizeof(g_vertex_buffer.data()), g_vertex_buffer.data(), GL_STATIC_DRAW);
    
    // One color for each vertex. They were generated randomly.
    static GLfloat g_color_buffer_data[34*3];
    for (int i=0; i<34*3; i++) {
        g_color_buffer_data[i] = float_rand( 0.0f, 1.0f);
    }
    
    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
    
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

    
    
    
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );
    
    // Get a handle for our "MVP" uniform
    // Only during the initialisation
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    
    // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 ) {

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
        glDrawArrays(GL_TRIANGLES, 0, 3*12); // Starting from vertex 0; 3 vertices total -> 1 triangle
        glDisableVertexAttribArray(0);
        
        

        
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
