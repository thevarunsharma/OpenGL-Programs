// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow *window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "shader.hpp"

int main(void)
{
    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(1024, 768, "Fold", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders("SimpleTransform.vertexshader", "SimpleFragmentShader.fragmentshader");

    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    // Or, for an ortho camera :
    //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

    static const GLfloat vertex1[] = {
        -2.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        -2.0f, 1.0f, 0.0f,
        -2.0f, 0.0f, 0.0f,
    };
    static const GLfloat vertex2[] = {
        -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
    };
    static const GLfloat vertex3[] = {
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
    };
    static const GLfloat vertex4[] = {
        1.0f, 0.0f, 0.0f,
        2.0f, 0.0f, 0.0f,
        2.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
    };

    GLuint vertexbuffer1, vertexbuffer2, vertexbuffer3, vertexbuffer4;
    glGenBuffers(1, &vertexbuffer1);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex1), vertex1, GL_STATIC_DRAW);

    glGenBuffers(1, &vertexbuffer2);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex2), vertex2, GL_STATIC_DRAW);

    glGenBuffers(1, &vertexbuffer3);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex3), vertex3, GL_STATIC_DRAW);

    glGenBuffers(1, &vertexbuffer4);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer4);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex4), vertex4, GL_STATIC_DRAW);

    glm::vec3 axis(0, -1, 0);
    float th1 = 0, th2 = 0;
    bool done = false;
    glm::mat4 MVP1, MVP2, MVP3, MVP4;
    do
    {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
        // Camera matrix
        glm::mat4 View = glm::lookAt(
            glm::vec3(0, 5, 5), // Camera is at (4,3,3), in World Space
            glm::vec3(0, 0, 0),   // and looks at the origin
            glm::vec3(0, 1, 0)    // Head is up (set to 0,-1,0 to look upside-down)
        );
        //rotate
        // Model matrix : an identity matrix (model will be at the origin)
        glm::mat4 Model = glm::mat4(1.0f);

        glm::mat4 translate1 = glm::translate(Model, glm::vec3(1, 0, 0));
        glm::mat4 translate2 = glm::translate(Model, glm::vec3(-1, 0, 0));

        glm::mat4 Rotate1 = glm::rotate(Model, glm::radians(-th1), axis);
        glm::mat4 Rotate2 = glm::rotate(Model, glm::radians(-th2), axis);
        glm::mat4 Rotate3 = glm::rotate(Model, glm::radians(th2), axis);
        glm::mat4 Rotate4 = glm::rotate(Model, glm::radians(th1), axis);
        
        if (th1==180){
            done = true;
        }
        if (!(th2==90 && done)){
            if (!done)
                th1 = min(th1 + 1, 180.0f);
            else{
                th2 = min(th2 + 1, 180.0f);
                th1 = th2;
            }
        }
        // Our ModelViewProjection : multiplication of our 3 matrices
        
        if (!done){
            MVP1 = Projection * View * translate2 * Rotate1 * translate1 * Model;
            MVP4 = Projection * View * translate1 * Rotate4 * translate2 * Model;
            MVP2 = Projection * View * Model;
            MVP3 = Projection * View * Model;
        }else{
            MVP1 = Projection * View * Rotate1 * translate1 * Model;
            MVP4 = Projection * View * Rotate4 * translate2 * Model;
            MVP2 = Projection * View * Rotate2 * Model;
            MVP3 = Projection * View * Rotate3 * Model;
        }

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP1[0][0]);
        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer1);
        glVertexAttribPointer(
            0,        // attribute. No particular reason for 0, but must match the layout in the shader.
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );

        // Draw the triangle !
        glDrawArrays(GL_LINE_LOOP, 0, 5); // 3 indices starting at 0 -> 1 triangle
        glDisableVertexAttribArray(0);
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);
        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
        glVertexAttribPointer(
            0,        // attribute. No particular reason for 0, but must match the layout in the shader.
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );

        // Draw the triangle !
        glDrawArrays(GL_LINE_LOOP, 0, 4); // 3 indices starting at 0 -> 1 triangle
        glDisableVertexAttribArray(0);

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP3[0][0]);
        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3);
        glVertexAttribPointer(
            0,        // attribute. No particular reason for 0, but must match the layout in the shader.
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );

        // Draw the triangle !
        glDrawArrays(GL_LINE_LOOP, 0, 5); // 3 indices starting at 0 -> 1 triangle
        glDisableVertexAttribArray(0);

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP4[0][0]);
        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer4);
        glVertexAttribPointer(
            0,        // attribute. No particular reason for 0, but must match the layout in the shader.
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );

        // Draw the triangle !
        glDrawArrays(GL_LINE_LOOP, 0, 5); // 3 indices starting at 0 -> 1 triangle

        glDisableVertexAttribArray(0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer1);
    glDeleteBuffers(1, &vertexbuffer2);
    glDeleteBuffers(1, &vertexbuffer3);
    glDeleteBuffers(1, &vertexbuffer4);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
