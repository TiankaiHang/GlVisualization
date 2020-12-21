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
#include "camera.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define PI 3.14159265358979323846

using namespace std;

#pragma comment(lib, "glfw3.lib")


// settings
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// count for image savings
int count_image = 0;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

void saveImage(const char* filepath, GLFWwindow* w);

void generateCubicData(vector<float>& Vertices, vector<int>& Indices);
void generateSphereData(vector<float>& Vertices, vector<int>& Indices);
void generateCubic_with_color(vector<float>& Vertices, vector<int>& Indices);

int draw_shape();
int draw_cubic();


// ====================================================================
//                        main function
// ====================================================================
int main()
{
    int flag = draw_shape();
    return flag;
}
// ====================================================================
// ====================================================================

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    // exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // w, s, d, a: to change the view point
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    // SPACE to save image
    bool tosave = true;
    string img_pth = "figs/sphere_lightning_";
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && tosave) {
        img_pth = img_pth + to_string(count_image) + ".png";
        saveImage(img_pth.c_str(), window);
        ++count_image;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void saveImage(const char* filepath, GLFWwindow* w) {
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

void generateCubicData(vector<float>& Vertices, vector<int>& Indices) {
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    for (int i = 0; i < 6 * 6 * 6; ++i)
        Vertices.push_back(vertices[i]);
    for (int i = 0; i < 6 * 6; ++i)
        Indices.push_back(i);
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
            float nx = x / RADIUS;
            float ny = y / RADIUS;
            float nz = z / RADIUS;
            Vertices.push_back(nx);
            Vertices.push_back(ny);
            Vertices.push_back(nz);

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

void generateCubic_with_color(vector<float>& Vertices, vector<int>& Indices) {
    const float length_half = 0.3;
    float vertices[] = {
        -length_half,  length_half,  length_half - length_half, 1.0f, 0.0f, 0.0f,
         length_half,  length_half,  length_half - length_half, 0.0f, 1.0f, 0.0f,
        -length_half, -length_half,  length_half - length_half, 0.0f, 0.0f, 1.0f,
         length_half, -length_half,  length_half - length_half, 1.0f, 0.0f, 0.0f,
        -length_half,  length_half, -length_half - length_half, 0.0f, 0.0f, 1.0f,
         length_half,  length_half, -length_half - length_half, 1.0f, 0.0f, 0.0f,
        -length_half, -length_half, -length_half - length_half, 0.0f, 1.0f, 0.0f,
         length_half, -length_half, -length_half - length_half, 0.0f, 0.0f, 1.0f,
    };
    int indices[] = {
        0, 2, 4, 0, 2, 1, 0, 4, 1, 3, 2, 7, 3, 2, 1, 3, 7, 1,
        5, 4, 1, 5, 7, 1, 5, 4, 7, 6, 4, 2, 6, 4, 7, 6, 2, 7,
    };

    for (int i = 0; i < 8 * 6; ++i)
        Vertices.push_back(vertices[i]);
    for (int j = 0; j < 36; ++j)
        Indices.push_back(indices[j]);
}

int draw_shape() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "drawn_by_tiankai", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("myShader/basic_lightning.vs", "myShader/basic_lightning.fs");
    Shader lightCubeShader("myShader/cubic_lightning.vs", "myShader/cubic_lightning.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    vector<float> Vertices;
    vector<int> Indices;

    // generateCubicData(Vertices, Indices);
    generateSphereData(Vertices, Indices);

    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO, EBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(float), &Vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(int), &Indices[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);


    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // cout << Vertices.size() << " " << Indices.size() << endl;


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // render the cube
        glBindVertexArray(cubeVAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);


        // also draw the lamp object
        //lightCubeShader.use();
        //lightCubeShader.setMat4("projection", projection);
        //lightCubeShader.setMat4("view", view);
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, lightPos);
        //model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        //lightCubeShader.setMat4("model", model);

        //glBindVertexArray(lightCubeVAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

int draw_cubic() {
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
    // generateSphereData(Vertices, Indices);
    // generateTriangleData(Vertices, Indices);
    generateCubic_with_color(Vertices, Indices);


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

    Shader ourShader("myShader/perShader.vs", "myShader/shader.fs");

    //while (!glfwWindowShouldClose(window)) {
    //    processInput(window);
    //    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //    //glUseProgram(shaderProgram);
    //    ourShader.use();
    //    ourShader.setFloat("someUniform", 1.0f);
    //    
    //    glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    //    glm::mat4 projection = glm::mat4(1.0f);
    //    glm::mat4 model = glm::mat4(1.0f);
    //    model = glm::rotate(model, glm::radians(-20.0f * 1), glm::vec3(1.0f, 0.5f, 0.2f));
    //    //projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    //    
    //    //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -1.0f));
    //    // pass transformation matrices to the shader
    //    ourShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    //    ourShader.setMat4("view", view);
    //    ourShader.setMat4("model", model);
    //    glBindVertexArray(VAO);

    //    //glEnable(GL_CULL_FACE);
    //    //glCullFace(GL_BACK);
    //    glEnable(GL_DEPTH_TEST);
    //    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
    //    //glDrawArrays(GL_TRIANGLES, 0, 36);
    //    glfwSwapBuffers(window);
    //    glfwPollEvents();
    //}

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //glUseProgram(shaderProgram);
        ourShader.use();
        ourShader.setFloat("someUniform", 1.0f);

        glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 projection = glm::mat4(1.0f);

        //projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -1.0f));

        // pass transformation matrices to the shader
        ourShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("view", view);

        glBindVertexArray(VAO);
        glEnable(GL_DEPTH_TEST);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //for (int i = 0; i < 18; ++i) {
        glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 model = glm::mat4(1.0f);
        float angle = 20.0f * 3;
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.5f, 0.2f));
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
        //}

        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_BACK);



        //glDrawArrays(GL_TRIANGLES, 0, 36);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    string savepath = "figs/cylinder_norm111_diff_color_ploygon.png";
    bool will_save = false;
    if (will_save)
        saveImage((char*)savepath.c_str(), window);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    //glDeleteProgram(shaderProgram);

    glfwTerminate();
}


// old version code
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow* window);
//void saveImage(char* filepath, GLFWwindow* w);
//void rotate_and_shift(float& _x, float& _y, float& _z, const vector<float>& rotate_mat, const vector<float>& offset);
//
//// generate data for two triangles
//void generateTriangleData(vector<float>& Vertices, vector<int>& Indices);
//// generate data for Cylinder
//void generateCylinderData(vector<float>& Vertices, vector<int>& Indices);
//// generate data for Sphere of a ball
//void generateSphereData(vector<float>& Vertices, vector<int>& Indices);
//// generate data for cubic
//void generateCubic(vector<float>& Vertices, vector<int>& Indices);
//
//int draw_cubic();
//
//
//const int SCREEN_WIDTH  = 800;
//const int SCREEN_HEIGHT = 800;
//
//// camera
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//// timing
//float deltaTime = 0.0f;
//float lastFrame = 0.0f;
//// lighting
//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
//
//
//// ============================================================
////                           GLSL
//// ============================================================
////const char* vertexShaderSource = "#version 330 core\n"
////    "layout (location = 0) in vec3 aPos;\n"
////    "void main()\n"
////    "{\n"
////    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
////    "}\0";
////const char* fragmentShaderSource = "#version 330 core\n"
////    "out vec4 FragColor;\n"
////    "void main()\n"
////    "{\n"
////    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
////    "}\n\0";
////const char* vertexShaderSource = "#version 330 core\n"
////    "layout (location = 0) in vec3 aPos;\n"
////    "layout (location = 1) in vec3 aColor;\n"
////    "out vec3 ourColor;\n"
////    "void main()"
////    "{\n"
////    "    gl_Position = vec4(aPos, 1.0);\n"
////    "    ourColor = aColor;\n"
////    "}\0";
////
////const char* fragmentShaderSource = "#version 330 core\n"
////    "out vec4 FragColor;\n"
////    "in vec3 ourColor;\n"
////    "void main()\n"
////    "{\n"
////    "    FragColor = vec4(ourColor, 1.0);\n"
////    "}\n\0";
//// ============================================================
//
//int main()
//{
//    int flag = draw_cubic();
//    return flag;
//}
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    glViewport(0, 0, width, height);
//}
//
//void processInput(GLFWwindow* window) {
//    // esc to exit
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//
//
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//        camera.ProcessKeyboard(FORWARD, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//        camera.ProcessKeyboard(BACKWARD, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//        camera.ProcessKeyboard(LEFT, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//        camera.ProcessKeyboard(RIGHT, deltaTime);
//}
//
//

//
//void rotate_and_shift(float& _x, float& _y, float& _z, const vector<float>& rotate_mat, const vector<float>& offset) {
//    float x_new = _x * rotate_mat[0] + _y * rotate_mat[1] + _z * rotate_mat[2] + offset[0];
//    float y_new = _x * rotate_mat[3] + _y * rotate_mat[4] + _z * rotate_mat[5] + offset[1];
//    float z_new = _x * rotate_mat[6] + _y * rotate_mat[7] + _z * rotate_mat[8] + offset[2];
//    _x = x_new;
//    _y = y_new;
//    _z = z_new;
//}
//
//void generateTriangleData(vector<float>& Vertices, vector<int>& Indices) {
//    float vertices[] = {
//         0.5f,  0.5f, 0.0f, 1.0f, 0.5f, 0.2f,  // top right
//         0.5f, -0.5f, 0.0f, 1.0f, 0.5f, 0.2f,  // bottom right
//        -0.5f, -0.5f, 0.0f, 1.0f, 0.5f, 0.2f,  // bottom left
//        -0.5f,  0.5f, 0.0f, 1.0f, 0.5f, 0.2f,  // top left 
//    };
//
//    int indices[] = {
//        0, 1, 3,  // first Triangle
//        1, 2, 3   // second Triangle
//    };
//    for (int i = 0; i < 24; ++i)
//        Vertices.push_back(vertices[i]);
//    for (int j = 0; j < 6; ++j)
//        Indices.push_back(indices[j]);
//}
//
//void generateCylinderData(vector<float>& Vertices, vector<int>& Indices) {
//    const float CYLINDER_RADIUS = 0.5;
//    const float CYLINDER_HEIGHT = 1.0;
//    const int CIRCLE_FACE_ANGLE_INTERVALS = 100;
//    const int CIRCLE_FACE_RADIUS_INTERVALS = 100;
//
//    const float CIRCLE_FACE_ANGLE_DELTA = 2.0 * PI / CIRCLE_FACE_ANGLE_INTERVALS;
//    const float CIRCLE_FACE_RADIUS_DELTA = CYLINDER_RADIUS / CIRCLE_FACE_RADIUS_INTERVALS;
//
//    const int SIDE_FACE_INTERVALS_X = 100;
//    const int SIDE_FACE_INTERVALS_Y = 100;
//    const float SIDE_FACE_ANGLE_DELTA = 2.0 * PI / SIDE_FACE_INTERVALS_X;
//    const float SIDE_FACE_HEIGHT_DELTA = CYLINDER_HEIGHT / SIDE_FACE_INTERVALS_Y;
//
//    vector<float> NORMAL = { 1.0, 1.0, 1.0 };
//    float ROTATE_ANGLE = 60.0 * PI / 180.0;
//    vector<float> OFFSET = { 0.0, 0.0, 0.0 };
//
//    //normalize the normal vector
//    float norm2OfVector = sqrt(NORMAL[0] * NORMAL[0] + NORMAL[1] * NORMAL[1] + NORMAL[2] * NORMAL[2]);
//    for (int i = 0; i < 3; ++i)
//        NORMAL[i] = NORMAL[i] * 1.0 / norm2OfVector;
//
//    //calculate the rotate matrix (Rodrigues' Rotation Formula)
//    //ref: https://sites.cs.ucsb.edu/~lingqi/teaching/resources/GAMES101_Lecture_04.pdf
//    vector<float> ROTATE_MATRIX;
//    ROTATE_MATRIX.resize(9);
//    ROTATE_MATRIX[0] = cos(ROTATE_ANGLE) + (1.0 - cos(ROTATE_ANGLE)) * NORMAL[0] * NORMAL[0] \
//        + sin(ROTATE_ANGLE) * 0.0;
//    ROTATE_MATRIX[1] = 0.0 + (1.0 - cos(ROTATE_ANGLE)) * NORMAL[0] * NORMAL[1] \
//        - sin(ROTATE_ANGLE) * NORMAL[2];
//    ROTATE_MATRIX[2] = 0.0 + (1.0 - cos(ROTATE_ANGLE)) * NORMAL[0] * NORMAL[2] \
//        + sin(ROTATE_ANGLE) * NORMAL[1];
//    ROTATE_MATRIX[3] = 0.0 + (1.0 - cos(ROTATE_ANGLE)) * NORMAL[1] * NORMAL[0] \
//        + sin(ROTATE_ANGLE) * NORMAL[2];
//    ROTATE_MATRIX[4] = cos(ROTATE_ANGLE) + (1.0 - cos(ROTATE_ANGLE)) * NORMAL[1] * NORMAL[1] \
//        + sin(ROTATE_ANGLE) * 0.0;
//    ROTATE_MATRIX[5] = 0.0 + (1.0 - cos(ROTATE_ANGLE)) * NORMAL[1] * NORMAL[2] \
//        - sin(ROTATE_ANGLE) * NORMAL[0];
//    ROTATE_MATRIX[6] = 0.0 + (1.0 - cos(ROTATE_ANGLE)) * NORMAL[2] * NORMAL[0] \
//        - sin(ROTATE_ANGLE) * NORMAL[1];
//    ROTATE_MATRIX[7] = 0.0 + (1.0 - cos(ROTATE_ANGLE)) * NORMAL[2] * NORMAL[1] \
//        + sin(ROTATE_ANGLE) * NORMAL[0];
//    ROTATE_MATRIX[8] = cos(ROTATE_ANGLE) + (1.0 - cos(ROTATE_ANGLE)) * NORMAL[2] * NORMAL[2] \
//        + sin(ROTATE_ANGLE) * 0.0;
//
//    //generate two circle faces
//    for (int i = 0; i <= CIRCLE_FACE_RADIUS_INTERVALS; ++i) {
//        for (int j = 0; j <= CIRCLE_FACE_ANGLE_INTERVALS; ++j) {
//            float _r = i * 1.0 * CIRCLE_FACE_RADIUS_DELTA;
//            float _theta = j * 1.0 * CIRCLE_FACE_ANGLE_DELTA;
//            float _x = _r * cos(_theta);
//            float _y = _r * sin(_theta);
//            float _z = CYLINDER_HEIGHT / 2.0;
//            rotate_and_shift(_x, _y, _z, ROTATE_MATRIX, OFFSET);
//            float _red = 0.8;
//            float _gre = 0.6;
//            float _blu = 0.7;
//            Vertices.push_back(_x);
//            Vertices.push_back(_y);
//            Vertices.push_back(_z);
//            Vertices.push_back(_red);
//            Vertices.push_back(_gre);
//            Vertices.push_back(_blu);
//            if (i != CIRCLE_FACE_RADIUS_INTERVALS && j != CIRCLE_FACE_ANGLE_INTERVALS) {
//                Indices.push_back(i * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j);
//                Indices.push_back((i + 1) * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j);
//                Indices.push_back((i + 1) * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + 1);
//                Indices.push_back(i * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j);
//                Indices.push_back((i + 1) * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + 1);
//                Indices.push_back(i * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + 1);
//            }
//        }
//    }
//
//    int inds_offset1 = (CIRCLE_FACE_RADIUS_INTERVALS + 1) * (CIRCLE_FACE_ANGLE_INTERVALS + 1);
//    for (int i = 0; i <= CIRCLE_FACE_RADIUS_INTERVALS; ++i) {
//        for (int j = 0; j <= CIRCLE_FACE_ANGLE_INTERVALS; ++j) {
//            float _r = i * 1.0 * CIRCLE_FACE_RADIUS_DELTA;
//            float _theta = j * 1.0 * CIRCLE_FACE_ANGLE_DELTA;
//            float _x = _r * cos(_theta);
//            float _y = _r * sin(_theta);
//            float _z = -CYLINDER_HEIGHT / 2.0;
//            float _red = 0.4;
//            float _gre = 0.4;
//            float _blu = 0.4;
//            rotate_and_shift(_x, _y, _z, ROTATE_MATRIX, OFFSET);
//            Vertices.push_back(_x);
//            Vertices.push_back(_y);
//            Vertices.push_back(_z);
//            Vertices.push_back(_red);
//            Vertices.push_back(_gre);
//            Vertices.push_back(_blu);
//            if (i != CIRCLE_FACE_RADIUS_INTERVALS && j != CIRCLE_FACE_ANGLE_INTERVALS) {
//                Indices.push_back(i * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + inds_offset1);
//                Indices.push_back((i + 1) * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + inds_offset1);
//                Indices.push_back((i + 1) * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + 1 + inds_offset1);
//                Indices.push_back(i * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + inds_offset1);
//                Indices.push_back((i + 1) * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + 1 + inds_offset1);
//                Indices.push_back(i * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + 1 + inds_offset1);
//            }
//        }
//    }
//
//    //generate side face
//    int inds_offset2 = (CIRCLE_FACE_RADIUS_INTERVALS + 1) * (CIRCLE_FACE_ANGLE_INTERVALS + 1) * 2;
//    for (int i = 0; i <= SIDE_FACE_INTERVALS_Y; ++i) {
//        for (int j = 0; j <= SIDE_FACE_INTERVALS_X; ++j) {
//            //float _r = i * 1.0 * CIRCLE_FACE_RADIUS_DELTA;
//            float _theta = j * 1.0 * SIDE_FACE_ANGLE_DELTA;
//            float _x = CYLINDER_RADIUS * cos(_theta);
//            float _y = CYLINDER_RADIUS * sin(_theta);
//            float _z = -CYLINDER_HEIGHT / 2 + i * SIDE_FACE_HEIGHT_DELTA;
//            float _red = 0.8;
//            float _gre = 0.6;
//            float _blu = 0.7;
//            rotate_and_shift(_x, _y, _z, ROTATE_MATRIX, OFFSET);
//            Vertices.push_back(_x);
//            Vertices.push_back(_y);
//            Vertices.push_back(_z);
//            Vertices.push_back(_red);
//            Vertices.push_back(_gre);
//            Vertices.push_back(_blu);
//            if (i != SIDE_FACE_INTERVALS_Y && j != SIDE_FACE_INTERVALS_X) {
//                Indices.push_back(i * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + inds_offset2);
//                Indices.push_back((i + 1) * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + inds_offset2);
//                Indices.push_back((i + 1) * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + 1 + inds_offset2);
//                Indices.push_back(i * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + inds_offset2);
//                Indices.push_back((i + 1) * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + 1 + inds_offset2);
//                Indices.push_back(i * (CIRCLE_FACE_ANGLE_INTERVALS + 1) + j + 1 + inds_offset2);
//            }
//        }
//    }
//}
//
//void generateSphereData(vector<float>& Vertices, vector<int>& Indices) {
//    const float RADIUS = 0.8;
//    const int INTERVALS = 50;
//    const float ANGLE_INTERVAL = 2.0 * PI / INTERVALS;
//    int index_point = 0;
//    for (int i = 0; i <= INTERVALS; ++i) {
//        for (int j = 0; j <= INTERVALS; ++j) {
//            float theta = i * 1.0 * ANGLE_INTERVAL;
//            float alpha = j * 1.0 * ANGLE_INTERVAL;
//            float x = RADIUS * cos(alpha) * cos(theta);
//            float y = RADIUS * cos(alpha) * sin(theta);
//            float z = RADIUS * sin(alpha);
//            //cout << x << " " << y << " " << z << endl;
//
//            Vertices.push_back(x);
//            Vertices.push_back(y);
//            Vertices.push_back(z);
//            float _red = 1.0;
//            float _gre = 0.5;
//            float _blu = 0.2;
//            Vertices.push_back(_red);
//            Vertices.push_back(_gre);
//            Vertices.push_back(_blu);
//
//            //CylinderIndices.push_back(index_point++);
//            //cout << index_point;
//            if (i != INTERVALS && j != INTERVALS) {
//                Indices.push_back(i * (INTERVALS + 1) + j);
//                Indices.push_back((i + 1) * (INTERVALS + 1) + j);
//                Indices.push_back((i + 1) * (INTERVALS + 1) + j + 1);
//                Indices.push_back(i * (INTERVALS + 1) + j);
//                Indices.push_back((i + 1) * (INTERVALS + 1) + j + 1);
//                Indices.push_back(i * (INTERVALS + 1) + j + 1);
//            }
//        }
//    }
//}
//
//void generateShadedBall(vector<float>& Vertices, vector<int>& Indices) {
//    // the function of the ball: x^2 + y^2 + z^2 = r^2
//    // x = r * cos(alpha) * cos(theta)
//    // y = r * cos(alpha) * sin(theta)
//    // z = r * sin(alpha)
//    // normal vector at (x, y, z) is (x / r, y / r, z / r)
//}
//

