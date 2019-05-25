#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
GLFWwindow *window;

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

    window = glfwCreateWindow(1024, 768, "Name", NULL, NULL);
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
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(20);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");

    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    static const GLfloat tetravertices[] = {
        0.0, 3.0, 0.0,
        1.5, 0.0, -1.0,
        -1.5, 0.0, -1.0,
        -1.5, 0.0, -1.0,
        0.0, 3.0, 0.0,
        0.0, 0.0, 2.0,
        0.0, 3.0, 0.0,
        0.0, 0.0, 2.0,
        1.5, 0.0, -1.0,
        1.5, 0.0, -1.0,
        -1.5, 0.0, -1.0,
        0.0, 0.0, 2.0,
    };

    static const GLfloat tetracolors[] = {
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,
        0.0, 1.0, 1.0,
        0.0, 1.0, 1.0,
        0.0, 1.0, 1.0
    };

    static const GLfloat numvertices[] = {
        -0.2, 2.5, 0.1,
        -0.7, 0.7, 0.33,
        0.1, 2.12, 0.7,
        0.4, 2.1, -0.2,
        0.4, 2.1, -0.2,
        0.85, 0.0, 0.6,
        -0.45, 1.8, -0.42,
        0.45, 1.8, -0.42,
        0.45, 1.8, -0.42,
        0.45, 1.2, -0.62,
        0.45, 1.2, -0.62,
        -0.45, 1.2, -0.62,
        -0.45, 1.2, -0.62,
        -0.45, 0.6, -0.82,
        -0.45, 0.6, -0.82,
        0.45, 0.6, -0.82,
        -0.4, -0.02, -0.8,
        0.4, -0.02, -0.8,
        0.4, -0.02, -0.8,
        0.4, -0.02, 0.2,
        0.4, -0.02, 0.2,
        -0.4, -0.02, 0.2,
        0.4, -0.02, 0.2,
        0.4, -0.02, 1.2,
        0.4, -0.02, 1.2,
        -0.4, -0.02, 1.2
    };

    static const GLfloat numcolors[] = {
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
    };

    GLuint vertexbuffer1;
    glGenBuffers(1, &vertexbuffer1);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tetravertices), tetravertices, GL_STATIC_DRAW);

    GLuint colorbuffer1;
    glGenBuffers(1, &colorbuffer1);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tetracolors), tetracolors, GL_STATIC_DRAW);

    GLuint vertexbuffer2;
    glGenBuffers(1, &vertexbuffer2);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(numvertices), numvertices, GL_STATIC_DRAW);

    GLuint colorbuffer2;
    glGenBuffers(1, &colorbuffer2);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(numcolors), numcolors, GL_STATIC_DRAW);

    float theta = 0.0f;
    glm::vec3 axis(0, 2, 3);
    do
    {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);
        // Projection
        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
        // Camera matrix
        glm::mat4 View = glm::lookAt(glm::vec3(1, 15, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
        //rotate
        // Model matrix : an identity matrix (model will be at the origin)
        glm::mat4 Model = glm::mat4(1.0f);
        glm::mat4 Rotate = glm::rotate(Model, glm::radians(theta), axis);
        theta += 1;
        // Our ModelViewProjection : multiplication of our 3 matrices
        glm::mat4 MVP = Projection * View * Rotate * Model; // Remember, matrix multiplication is the other way around

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer1);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 4*3);
        
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer2);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

        // Draw the triangle !
        glDrawArrays(GL_LINES, 0, 2*13);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer1);
    glDeleteBuffers(1, &colorbuffer1);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}