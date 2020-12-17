// Code for INFORMATION VISUALIZATION TECHNOLOGY
// written by Tiankai Hang (tkhang@seu.edu.cn)
// some code was borrowed from https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/
// 2020/10/06

// function: Draw Cylinder based on GLFW and GLAD

// #include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cmath>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define PI 3.14159265358979323846

using namespace std;

#pragma comment(lib, "glfw3.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void saveImage(char* filepath, GLFWwindow* w);
void rotate_and_shift(float& _x, float& _y, float& _z, const vector<float>& rotate_mat, const vector<float>& offset);

// generate data for two triangles
void generateTriangleData(vector<float>& Vertices, vector<int>& Indices);
// generate data for Cylinder
void generateCylinderData(vector<float>& Vertices, vector<int>& Indices);
// generate data for Sphere of a ball
void generateSphereData(vector<float>& Vertices, vector<int>& Indices);

const int SCREEN_WIDTH  = 800;
const int SCREEN_HEIGHT = 800;


// ============================================================
//                           GLSL
// ============================================================
//const char* vertexShaderSource = "#version 330 core\n"
//    "layout (location = 0) in vec3 aPos;\n"
//    "void main()\n"
//    "{\n"
//    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//    "}\0";
//const char* fragmentShaderSource = "#version 330 core\n"
//    "out vec4 FragColor;\n"
//    "void main()\n"
//    "{\n"
//    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
//    "}\n\0";
//const char* vertexShaderSource = "#version 330 core\n"
//    "layout (location = 0) in vec3 aPos;\n"
//    "layout (location = 1) in vec3 aColor;\n"
//    "out vec3 ourColor;\n"
//    "void main()"
//    "{\n"
//    "    gl_Position = vec4(aPos, 1.0);\n"
//    "    ourColor = aColor;\n"
//    "}\0";
//
//const char* fragmentShaderSource = "#version 330 core\n"
//    "out vec4 FragColor;\n"
//    "in vec3 ourColor;\n"
//    "void main()\n"
//    "{\n"
//    "    FragColor = vec4(ourColor, 1.0);\n"
//    "}\n\0";
// ============================================================

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Draw Cylinder", NULL, NULL);
    if (window == NULL) {
        cout << "Fail to create the window." << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Fail to initialize GLAD" << endl;
        return -1;
    }

    //vertex shader
    //int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    //glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    //glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    //if (!success)
    //{
    //    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    //    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    //}


    //fragment shader
    //int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    //glCompileShader(fragmentShader);
    //glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    //if (!success)
    //{
    //    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    //    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    //}

    //int shaderProgram = glCreateProgram();
    //glAttachShader(shaderProgram, vertexShader);
    //glAttachShader(shaderProgram, fragmentShader);
    //glLinkProgram(shaderProgram);
    //glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    //if (!success) {
    //    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    //    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    //}

    //glDeleteShader(vertexShader);
    //glDeleteShader(fragmentShader);

    vector<float> Vertices;
    vector<int> Indices;

    // generateCylinderData(Vertices, Indices);
    generateSphereData(Vertices, Indices);
    // generateTriangleData(Vertices, Indices);


    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(float), &Vertices[0], GL_STATIC_DRAW); //cylinder
    //glBufferData(GL_ARRAY_BUFFER, SphereVertices.size() * sizeof(float), &SphereVertices[0], GL_STATIC_DRAW); //sphere
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //triangle
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(int), &Indices[0], GL_STATIC_DRAW);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, SphereIndices.size() * sizeof(int), &SphereIndices[0], GL_STATIC_DRAW); //sphere
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //triangle

    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Shader ourShader("perShader.vs", "shader.fs");

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //glUseProgram(shaderProgram);
        ourShader.use();
        ourShader.setFloat("someUniform", 1.0f);
        
        glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 projection = glm::mat4(1.0f);
        
        projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
        // pass transformation matrices to the shader
        ourShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("view", view);
        
        glBindVertexArray(VAO);
        for (int i = 0; i < 18; ++i) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            ourShader.setMat4("model", model);
            //glBindVertexArray(VAO);

            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
        }
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    string savepath = "figs/cylinder_norm111_diff_color_ploygon.png";
    bool will_save = false;
    if(will_save)
        saveImage((char*)savepath.c_str(), window);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    //glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    // esc to exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


void saveImage(char* filepath, GLFWwindow* w) {
    //ref: https://lencerf.github.io/post/2019-09-21-save-the-opengl-rendering-to-image-file/
    int width, height;
    glfwGetFramebufferSize(w, &width, &height);
    GLsizei nrChannels = 3;
    GLsizei stride = nrChannels * width;
    stride += (stride % 4) ? (4 - stride % 4) : 0;
    GLsizei bufferSize = stride * height;
    std::vector<char> buffer(bufferSize);
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());
    stbi_flip_vertically_on_write(true);
    stbi_write_png(filepath, width, height, nrChannels, buffer.data(), stride);
}

void rotate_and_shift(float& _x, float& _y, float& _z, const vector<float>& rotate_mat, const vector<float>& offset) {
    float x_new = _x * rotate_mat[0] + _y * rotate_mat[1] + _z * rotate_mat[2] + offset[0];
    float y_new = _x * rotate_mat[3] + _y * rotate_mat[4] + _z * rotate_mat[5] + offset[1];
    float z_new = _x * rotate_mat[6] + _y * rotate_mat[7] + _z * rotate_mat[8] + offset[2];
    _x = x_new;
    _y = y_new;
    _z = z_new;
}

void generateTriangleData(vector<float>& Vertices, vector<int>& Indices) {
    float vertices[] = {
         0.5f,  0.5f, 0.0f, 1.0f, 0.5f, 0.2f,  // top right
         0.5f, -0.5f, 0.0f, 1.0f, 0.5f, 0.2f,  // bottom right
        -0.5f, -0.5f, 0.0f, 1.0f, 0.5f, 0.2f,  // bottom left
        -0.5f,  0.5f, 0.0f, 1.0f, 0.5f, 0.2f,  // top left 
    };

    int indices[] = {
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    for (int i = 0; i < 24; ++i)
        Vertices.push_back(vertices[i]);
    for (int j = 0; j < 6; ++j)
        Indices.push_back(indices[j]);
}

void generateCylinderData(vector<float>& Vertices, vector<int>& Indices) {
    const float CYLINDER_RADIUS = 0.5;
    const float CYLINDER_HEIGHT = 1.0;
    const int CIRCLE_FACE_ANGLE_INTERVALS = 100;
    const int CIRCLE_FACE_RADIUS_INTERVALS = 100;

    const float CIRCLE_FACE_ANGLE_DELTA = 2.0 * PI / CIRCLE_FACE_ANGLE_INTERVALS;
    const float CIRCLE_FACE_RADIUS_DELTA = CYLINDER_RADIUS / CIRCLE_FACE_RADIUS_INTERVALS;

    const int SIDE_FACE_INTERVALS_X = 100;
    const int SIDE_FACE_INTERVALS_Y = 100;
    const float SIDE_FACE_ANGLE_DELTA = 2.0 * PI / SIDE_FACE_INTERVALS_X;
    const float SIDE_FACE_HEIGHT_DELTA = CYLINDER_HEIGHT / SIDE_FACE_INTERVALS_Y;

    vector<float> NORMAL = { 1.0, 1.0, 1.0 };
    float ROTATE_ANGLE = 60.0 * PI / 180.0;
    vector<float> OFFSET = { 0.0, 0.0, 0.0 };

    //normalize the normal vector
    float norm2OfVector = sqrt(NORMAL[0] * NORMAL[0] + NORMAL[1] * NORMAL[1] + NORMAL[2] * NORMAL[2]);
    for (int i = 0; i < 3; ++i)
        NORMAL[i] = NORMAL[i] * 1.0 / norm2OfVector;

    //calculate the rotate matrix (Rodrigues' Rotation Formula)
    //ref: https://sites.cs.ucsb.edu/~lingqi/teaching/resources/GAMES101_Lecture_04.pdf
    vector<float> ROTATE_MATRIX;
    ROTATE_MATRIX.resize(9);
    ROTATE_MATRIX[0] = cos(ROTATE_ANGLE) + (1.0 - cos(ROTATE_ANGLE)) * NORMAL[0] * NORMAL[0] \
        + sin(ROTATE_ANGLE) * 0.0;
    ROTATE_MATRIX[1] = 0.0 + (1.0 - cos(ROTATE_ANGLE)) * NORMAL[0] * NORMAL[1] \
        - sin(ROTATE_ANGLE) * NORMAL[2];
    ROTATE_MATRIX[2] = 0.0 + (1.0 - cos(ROTATE_ANGLE)) * NORMAL[0] * NORMAL[2] \
        + sin(ROTATE_ANGLE) * NORMAL[1];
    ROTATE_MATRIX[3] = 0.0 + (1.0 - cos(ROTATE_ANGLE)) * NORMAL[1] * NORMAL[0] \
        + sin(ROTATE_ANGLE) * NORMAL[2];
    ROTATE_MATRIX[4] = cos(ROTATE_ANGLE) + (1.0 - cos(ROTATE_ANGLE)) * NORMAL[1] * NORMAL[1] \
        + sin(ROTATE_ANGLE) * 0.0;
    ROTATE_MATRIX[5] = 0.0 + (1.0 - cos(ROTATE_ANGLE)) * NORMAL[1] * NORMAL[2] \
        - sin(ROTATE_ANGLE) * NORMAL[0];
    ROTATE_MATRIX[6] = 0.0 + (1.0 - cos(ROTATE_ANGLE)) * NORMAL[2] * NORMAL[0] \
        - sin(ROTATE_ANGLE) * NORMAL[1];
    ROTATE_MATRIX[7] = 0.0 + (1.0 - cos(ROTATE_ANGLE)) * NORMAL[2] * NORMAL[1] \
        + sin(ROTATE_ANGLE) * NORMAL[0];
    ROTATE_MATRIX[8] = cos(ROTATE_ANGLE) + (1.0 - cos(ROTATE_ANGLE)) * NORMAL[2] * NORMAL[2] \
        + sin(ROTATE_ANGLE) * 0.0;

    //generate two circle faces
    for (int i = 0; i <= CIRCLE_FACE_RADIUS_INTERVALS; ++i) {
        for (int j = 0; j <= CIRCLE_FACE_ANGLE_INTERVALS; ++j) {
            float _r = i * 1.0 * CIRCLE_FACE_RADIUS_DELTA;
            float _theta = j * 1.0 * CIRCLE_FACE_ANGLE_DELTA;
            float _x = _r * cos(_theta);
            float _y = _r * sin(_theta);
            float _z = CYLINDER_HEIGHT / 2.0;
            rotate_and_shift(_x, _y, _z, ROTATE_MATRIX, OFFSET);
            float _red = 0.8;
            float _gre = 0.6;
            float _blu = 0.7;
            Vertices.push_back(_x);
            Vertices.push_back(_y);
            Vertices.push_back(_z);
            Vertices.push_back(_red);
            Vertices.push_back(_gre);
            Vertices.push_back(_blu);
            if (i != CIRCLE_FACE_RADIUS_INTERVALS && j != CIRCLE_FACE_ANGLE_INTERVALS) {
                Indices.push_back(i * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j);
                Indices.push_back((i + 1) * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j);
                Indices.push_back((i + 1) * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + 1);
                Indices.push_back(i * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j);
                Indices.push_back((i + 1) * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + 1);
                Indices.push_back(i * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + 1);
            }
        }
    }

    int inds_offset1 = (CIRCLE_FACE_RADIUS_INTERVALS + 1) * (CIRCLE_FACE_ANGLE_INTERVALS + 1);
    for (int i = 0; i <= CIRCLE_FACE_RADIUS_INTERVALS; ++i) {
        for (int j = 0; j <= CIRCLE_FACE_ANGLE_INTERVALS; ++j) {
            float _r = i * 1.0 * CIRCLE_FACE_RADIUS_DELTA;
            float _theta = j * 1.0 * CIRCLE_FACE_ANGLE_DELTA;
            float _x = _r * cos(_theta);
            float _y = _r * sin(_theta);
            float _z = -CYLINDER_HEIGHT / 2.0;
            float _red = 0.4;
            float _gre = 0.4;
            float _blu = 0.4;
            rotate_and_shift(_x, _y, _z, ROTATE_MATRIX, OFFSET);
            Vertices.push_back(_x);
            Vertices.push_back(_y);
            Vertices.push_back(_z);
            Vertices.push_back(_red);
            Vertices.push_back(_gre);
            Vertices.push_back(_blu);
            if (i != CIRCLE_FACE_RADIUS_INTERVALS && j != CIRCLE_FACE_ANGLE_INTERVALS) {
                Indices.push_back(i * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + inds_offset1);
                Indices.push_back((i + 1) * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + inds_offset1);
                Indices.push_back((i + 1) * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + 1 + inds_offset1);
                Indices.push_back(i * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + inds_offset1);
                Indices.push_back((i + 1) * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + 1 + inds_offset1);
                Indices.push_back(i * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + 1 + inds_offset1);
            }
        }
    }

    //generate side face
    int inds_offset2 = (CIRCLE_FACE_RADIUS_INTERVALS + 1) * (CIRCLE_FACE_ANGLE_INTERVALS + 1) * 2;
    for (int i = 0; i <= SIDE_FACE_INTERVALS_Y; ++i) {
        for (int j = 0; j <= SIDE_FACE_INTERVALS_X; ++j) {
            //float _r = i * 1.0 * CIRCLE_FACE_RADIUS_DELTA;
            float _theta = j * 1.0 * SIDE_FACE_ANGLE_DELTA;
            float _x = CYLINDER_RADIUS * cos(_theta);
            float _y = CYLINDER_RADIUS * sin(_theta);
            float _z = -CYLINDER_HEIGHT / 2 + i * SIDE_FACE_HEIGHT_DELTA;
            float _red = 0.8;
            float _gre = 0.6;
            float _blu = 0.7;
            rotate_and_shift(_x, _y, _z, ROTATE_MATRIX, OFFSET);
            Vertices.push_back(_x);
            Vertices.push_back(_y);
            Vertices.push_back(_z);
            Vertices.push_back(_red);
            Vertices.push_back(_gre);
            Vertices.push_back(_blu);
            if (i != SIDE_FACE_INTERVALS_Y && j != SIDE_FACE_INTERVALS_X) {
                Indices.push_back(i * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + inds_offset2);
                Indices.push_back((i + 1) * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + inds_offset2);
                Indices.push_back((i + 1) * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + 1 + inds_offset2);
                Indices.push_back(i * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + inds_offset2);
                Indices.push_back((i + 1) * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + 1 + inds_offset2);
                Indices.push_back(i * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + 1 + inds_offset2);
            }
        }
    }
}

void generateSphereData(vector<float>& Vertices, vector<int>& Indices) {
    const float RADIUS = 0.8;
    const int INTERVALS = 50;
    const float ANGLE_INTERVAL = 2.0 * PI / INTERVALS;
    int index_point = 0;
    for (int i = 0; i <= INTERVALS; ++i) {
        for (int j = 0; j <= INTERVALS; ++j) {
            float theta = i * 1.0 * ANGLE_INTERVAL;
            float alpha = j * 1.0 * ANGLE_INTERVAL;
            float x = RADIUS * cos(alpha) * cos(theta);
            float y = RADIUS * cos(alpha) * sin(theta);
            float z = RADIUS * sin(alpha);
            //cout << x << " " << y << " " << z << endl;

            Vertices.push_back(x);
            Vertices.push_back(y);
            Vertices.push_back(z);
            float _red = 1.0;
            float _gre = 0.5;
            float _blu = 0.2;
            Vertices.push_back(_red);
            Vertices.push_back(_gre);
            Vertices.push_back(_blu);

            //CylinderIndices.push_back(index_point++);
            //cout << index_point;
            if (i != INTERVALS && j != INTERVALS) {
                Indices.push_back(i * (INTERVALS + 1) + j);
                Indices.push_back((i + 1) * (INTERVALS + 1) + j);
                Indices.push_back((i + 1) * (INTERVALS + 1) + j + 1);
                Indices.push_back(i * (INTERVALS + 1) + j);
                Indices.push_back((i + 1) * (INTERVALS + 1) + j + 1);
                Indices.push_back(i * (INTERVALS + 1) + j + 1);
            }
        }
    }
}

void generateShadedBall(vector<float>& Vertices, vector<int>& Indices) {
    // the function of the ball: x^2 + y^2 + z^2 = r^2
    // x = r * cos(alpha) * cos(theta)
    // y = r * cos(alpha) * sin(theta)
    // z = r * sin(alpha)
    // normal vector at (x, y, z) is (x / r, y / r, z / r)
}