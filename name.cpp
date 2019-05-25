#include <stdio.h>
#include <stdlib.h>
#include<vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
GLFWwindow *window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "shader.hpp"

long int nCr(int n, int r){
    long int num = 1, den = 1;
    for (int i = n-r+1; i <= n; i++)
        num *= i;
    for (int i = 1; i <= r; i++)
        den *= i;
    return num/den;
}

void BezierPoints(int n, std::vector<std::pair<float, float>> P, std::vector<std::pair<float, float>> &points){
    double t = 0, cosa, sina;
    float x, y, dx, dy;
    double mul;
    int j = 0; 
    while (t <= 1){
        x = 0; y = 0; dx = 0; dy = 0; 
        for (int i=0; i<n; i++){
            mul = nCr(n-1, i) * pow(1-t, n-i-1) * pow(t, i);
            x += mul * P[i].first;
            y += mul * P[i].second;
            if (!((t == 0 && i == 0) || (i == n-1 && t == 1))){
                dx += P[i].first * nCr(n-1, i) * (i - (n-1)*t) * pow(1-t, n-i-2) * pow(t, i-1);
                dy += P[i].second * nCr(n-1, i) * (i - (n-1)*t) * pow(1-t, n-i-2) * pow(t, i-1);
            }
        }
        cosa = dy/pow(dx*dx + dy*dy, 0.5);
        sina = -dx/pow(dx*dx + dy*dy, 0.5);
        points.push_back(std::make_pair(x, y));
        j += 1;
        t += 0.01;
    }
}

int main(void){
    // Initialise GLFW
    if (!glfwInit()){
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

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders("SimpleTransform.vertexshader", "SimpleFragmentShader.fragmentshader");

    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    // static const GLfloat vertices[]={
    //     0, 2, 0,
    //     1, 0, 0,
    //     1, 0, 0,
    //     2, 2, 0,
    //     2, 0, 0,
    //     3, 2, 0,
    //     3, 2, 0,
    //     4, 0, 0,
    //     2.5, 1, 0,
    //     3.5, 1, 0,
    //     4.5, 0, 0,
    //     4.5, 2, 0,
    //     4.5, 2, 0,
    //     5.5, 2, 0,
    //     5.5, 2, 0,
    //     5.5, 1, 0,
    //     5.5, 1, 0,
    //     4.5, 1, 0,
    //     5, 1, 0,
    //     5.5, 0, 0,
    //     6, 0, 0,
    //     6, 2, 0,
    //     6, 0, 0,
    //     7, 0, 0,
    //     7, 0, 0,
    //     7, 2, 0,
    //     7.5, 0, 0,
    //     7.5, 2, 0,
    //     7.5, 2, 0,
    //     8.5, 0, 0,
    //     8.5, 0, 0,
    //     8.5, 2, 0
    // };

    std::vector<std::pair<float, float>> V1, Vd;
    V1.push_back(std::make_pair(0, 1));
    V1.push_back(std::make_pair(0.2, 2.9));
    V1.push_back(std::make_pair(0.4, 2.6));
    V1.push_back(std::make_pair(1, 0));
    BezierPoints(V1.size(), V1, Vd);

    std::vector<std::pair<float, float>> V2;
    V2.push_back(std::make_pair(1, 0));
    V2.push_back(std::make_pair(1.8, 2.7));
    BezierPoints(V2.size(), V2, Vd);

    std::vector<std::pair<float, float>> a1;
    a1.push_back(std::make_pair(3., 1.4));
    a1.push_back(std::make_pair(2, 1.6));
    a1.push_back(std::make_pair(1.7, 0.8));
    a1.push_back(std::make_pair(0.5, -1));
    a1.push_back(std::make_pair(3.8, -1));
    a1.push_back(std::make_pair(3.3, 0.7));
    a1.push_back(std::make_pair(2.9, 0.9));
    a1.push_back(std::make_pair(4.2, 1.7));
    a1.push_back(std::make_pair(2.2, 2.6));
    a1.push_back(std::make_pair(3.4, -0.3));
    BezierPoints(a1.size(), a1, Vd);

    std::vector<std::pair<float, float>> r1;
    r1.push_back(std::make_pair(3.7, 0.5));
    r1.push_back(std::make_pair(4.2, 1.5));
    r1.push_back(std::make_pair(3.7, 2.3));
    r1.push_back(std::make_pair(2.7, 0.5));
    r1.push_back(std::make_pair(4.5, 1.3));
    BezierPoints(r1.size(), r1, Vd);

    std::vector<std::pair<float, float>> r2;
    r2.push_back(std::make_pair(4.5, 1.3));
    r2.push_back(std::make_pair(3.7, 0.2));
    r2.push_back(std::make_pair(4.7, 0));
    BezierPoints(r2.size(), r2, Vd);

    std::vector<std::pair<float, float>> u1;
    u1.push_back(std::make_pair(5, 1.3));
    u1.push_back(std::make_pair(4.5, -0.5));
    u1.push_back(std::make_pair(5.2, -0.3));
    u1.push_back(std::make_pair(6.2, 0.3));
    u1.push_back(std::make_pair(5.3, 0.7));
    u1.push_back(std::make_pair(7, 2));
    u1.push_back(std::make_pair(5.2, 2));
    u1.push_back(std::make_pair(6.1, -0.1));
    BezierPoints(u1.size(), u1, Vd);

    std::vector<std::pair<float, float>> n1;
    n1.push_back(std::make_pair(6.4, 1));
    n1.push_back(std::make_pair(7, 2));
    n1.push_back(std::make_pair(6.5, 0));
    BezierPoints(n1.size(), n1, Vd);

    std::vector<std::pair<float, float>> n2;
    n2.push_back(std::make_pair(6.5, 0));
    n2.push_back(std::make_pair(6.7, 2));
    n2.push_back(std::make_pair(8.3, 1.8));
    n2.push_back(std::make_pair(7.9, 1));
    n2.push_back(std::make_pair(7.1, 0.24));
    n2.push_back(std::make_pair(7.1, -0.4));
    n2.push_back(std::make_pair(8.7, 0.35));
    BezierPoints(n2.size(), n2, Vd);

    int n = (Vd.size())*3;
    GLfloat vertices[n];
    for (int i=0; i<Vd.size(); i++){
        float x = Vd[i].first, y = Vd[i].second;
        vertices[3*i] = x;
        vertices[3*i+1] = y;
        vertices[3*i+2] = 0.0f;
    }

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    float theta = 0.0f;
    glm::vec3 axis(0, 1, 0);
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
            glm::vec3(-1, 5, 20), // Camera is at (4,3,3), in World Space
            glm::vec3(0, 0, 0),   // and looks at the origin
            glm::vec3(0, 1, 0)    // Head is up (set to 0,-1,0 to look upside-down)
        );
        //rotate
        // Model matrix : an identity matrix (model will be at the origin)
        glm::mat4 Model = glm::mat4(1.0f);
        glm::mat4 Translate = glm::translate(Model, glm::vec3(-3, 0, 0));
        glm::mat4 Rotate = glm::rotate(Model, glm::radians(theta), axis);
        theta += 1;
        // Our ModelViewProjection : multiplication of our 3 matrices
        glm::mat4 MVP = Projection * View * Rotate * Translate * Model; // Remember, matrix multiplication is the other way around

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,        // attribute. No particular reason for 0, but must match the layout in the shader.
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );

        glDrawArrays(GL_LINE_STRIP, 0, n/3); // 3 indices starting at 0 -> 1 triangle

        glDisableVertexAttribArray(0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}