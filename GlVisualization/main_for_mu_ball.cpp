#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
using namespace std;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 SetColor;\n"
"void main()"
"{\n"
"    gl_Position = vec4(aPos, 1.0);\n"
"    SetColor = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 OutFragColor;\n"
"in vec3 SetColor;\n"
"void main()\n"
"{\n"
"    OutFragColor = vec4(SetColor, 1.0);\n"
"}\0";

const float PI = 3.1415926;

int main()
{
    ifstream verticesFile("CodeForMu/ball_vertices.txt");
    ifstream indsFile("CodeForMu/ball_inds.txt");

    vector<float> Vertices;
    vector<int> Indices;
    char filebuffer[256];
    float x, y, z, r, g, b;
    int count_vertices = 0;
    int count_inds = 0;
    int index = 0;

    if (!verticesFile) {
        cout << "Fail to open the vertices file!" << endl;
        exit(1);
    }
    if (!indsFile) {
        cout << "Fail to open the inds file!" << endl;
    }

    while (!verticesFile.eof()) {
        verticesFile.getline(filebuffer, 100);
        sscanf_s(filebuffer, "%f,%f,%f,%f,%f,%f", &x, &y, &z, &r, &g, &b);
        //if (count_vertices < 10302) {
        //    r = 0.1; g = 0.2; b = 0.3;
        //}
        //else {
        //    r = 0.1; g = 0.6; b = 0.3;
        //}
        ++count_vertices;
        Vertices.push_back(x); Vertices.push_back(y); Vertices.push_back(z);
        Vertices.push_back(r); Vertices.push_back(g); Vertices.push_back(b);
    }
    verticesFile.close();
    //cout << Vertices.size() << " " << Indices.size() << endl;
    while (!indsFile.eof()) {
        indsFile.getline(filebuffer, 100);
        sscanf_s(filebuffer, "%d", &index);
        Indices.push_back(index);
        ++count_inds;
    }
    indsFile.close();

    //cout << Vertices.size() << " " << Indices.size() << endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const int SR_WIDTH = 800;
    const int SR_HEIGHT = 800;

    GLFWwindow* window = glfwCreateWindow(SR_WIDTH, SR_HEIGHT, "PROJECT ONE", NULL, NULL);
    if (window == NULL) {
        cout << "Fail to create the window." << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Fail to initialize GLAD" << endl;
        return -1;
    }

    //int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    //glCompileShader(vertexShader);

    //int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    //glCompileShader(fragmentShader);

    //int shaderProgram = glCreateProgram();
    //glAttachShader(shaderProgram, vertexShader);
    //glAttachShader(shaderProgram, fragmentShader);
    //glLinkProgram(shaderProgram);

    //glDeleteShader(vertexShader);
    //glDeleteShader(fragmentShader);

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(float), &Vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(int), &Indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);

    Shader ball_lightning_shader("myShader/basic_lightning.vs", "myShader/basic_lightning.fs");

    //glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ball_lightning_shader.use();
        //glUseProgram(shaderProgram);
        //glBindVertexArray(VAO);

        glm::vec3 viewPos(0.0f, 0.0f, 3.0f);
        glm::vec3 lightPos(-1.0f, 0.0f, 2.0f);
        glm::mat4 projection = glm::perspective(60.0f / 180.0f * PI, (GLfloat)SR_WIDTH / SR_HEIGHT, 0.1f, 10.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(-1.0f, 1.0f, 3.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 model = glm::mat4(1.0f);
        // glm::mat4 view = glm::mat4(1.0f);
        glm::vec3 lightColor(1.0, 1.0, 1.0);
        glm::vec3 objectColor(0.4, 0.6, 0.3);

        ball_lightning_shader.setVec3("viewPos", viewPos);
        ball_lightning_shader.setVec3("lightPos", lightPos);
        ball_lightning_shader.setMat4("model", model);
        ball_lightning_shader.setMat4("view", view);
        ball_lightning_shader.setMat4("projection", projection);
        ball_lightning_shader.setVec3("lightColor", lightColor);
        ball_lightning_shader.setVec3("objectColor", objectColor);

        glEnable(GL_DEPTH_TEST);
        //glEnable(GL_CULL_FACE);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //glCullFace(GL_BACK);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    //glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}