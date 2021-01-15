#pragma once
#ifndef VOLUME_RENDERING_UTILS
#define VOLUME_RENDERING_UTILS

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
#include "volume.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#pragma comment(lib, "glfw3.lib")

#define PI 3.14159265358979323846
#define DRAW_CUBIC 0
#define DRAW_SPHERE 1
#define HEAD_RENDERING 2
#define GEN_RENDERING 3

using namespace std;

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
GLuint g_frameBuffer;

// defined functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void saveImage(const char* filepath, GLFWwindow* w);

void generateCubicData(vector<float>& Vertices, vector<int>& Indices);
void generateSphereData(vector<float>& Vertices, vector<int>& Indices);
void generateCubic_with_color(vector<float>& Vertices, vector<int>& Indices);
void generateCubic_for_vr(vector<float>& Vertices, vector<int>& Indices);

GLuint initTFF1DTex(const char* filename);
GLuint initFace2DTex(GLuint bfTexWidth, GLuint bfTexHeight);
GLuint initVol3DTex(const char* filename, GLuint w, GLuint h, GLuint d);
GLuint initVol3DTexFromGeneratedData();

void initFrameBuffer(GLuint, GLuint, GLuint);
void checkFramebufferStatus();

int draw_shape(int shape_flag);
int draw_cubic();
int VolumeRendering(int volume_choice);

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

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
    string img_pth = "figs/volume_rendering_";
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && tosave) {
        img_pth = img_pth + to_string(count_image) + ".png";
        saveImage(img_pth.c_str(), window);
        ++count_image;
    }
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

// save image
// ----------------------------------------------------------------------
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

// generate cubic data
// ----------------------------------------------------------------------
void generateCubicData(vector<float>& Vertices, vector<int>& Indices) {
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,//
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,//
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,//
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,//
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,//
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

void generateCubic_for_vr(vector<float>& Vertices, vector<int>& Indices) {
    GLfloat vertices[24] = {
    0.0, 0.0, 0.0,
    0.0, 0.0, 1.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 1.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 1.0,
    1.0, 1.0, 0.0,
    1.0, 1.0, 1.0
    };
    GLuint indices[36] = {
    1,5,7,
    7,3,1,
    0,2,6,
    6,4,0,
    0,1,3,
    3,2,0,
    7,5,4,
    4,6,7,
    2,3,7,
    7,6,2,
    1,0,4,
    4,5,1
    };

    for (int i = 0; i < 24; ++i)
        Vertices.push_back(vertices[i]);
    for (int i = 0; i < 36; ++i)
        Indices.push_back(indices[i]);
}

GLuint initTFF1DTex(const char* filename)
{
    // read in the user defined data of transfer function
    ifstream inFile(filename, ifstream::in);
    if (!inFile)
    {
        cerr << "Error openning file: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    const int MAX_CNT = 10000;
    GLubyte* tff = (GLubyte*)calloc(MAX_CNT, sizeof(GLubyte));
    inFile.read(reinterpret_cast<char*>(tff), MAX_CNT);
    if (inFile.eof())
    {
        size_t bytecnt = inFile.gcount();
        *(tff + bytecnt) = '\0';
        cout << "bytecnt " << bytecnt << endl;
    }
    else if (inFile.fail())
    {
        cout << filename << "read failed " << endl;
    }
    else
    {
        cout << filename << "is too large" << endl;
    }
    GLuint tff1DTex;
    glGenTextures(1, &tff1DTex);
    glBindTexture(GL_TEXTURE_1D, tff1DTex);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, tff);

    // print tff
    //for (int i = 0; i < MAX_CNT; ++i) {
    //    cout << (int)tff[i] << " ";
    //}
    free(tff);
    return tff1DTex;
}

GLuint initFace2DTex(GLuint bfTexWidth, GLuint bfTexHeight)
{
    GLuint backFace2DTex;
    glGenTextures(1, &backFace2DTex);
    glBindTexture(GL_TEXTURE_2D, backFace2DTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, bfTexWidth, bfTexHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    return backFace2DTex;
}

GLuint initVol3DTex(const char* filename, GLuint w, GLuint h, GLuint d)
{

    FILE* fp;
    size_t size = w * h * d;
    GLubyte* data = new GLubyte[size];			  // 8bit
    if (!(fp = fopen(filename, "rb")))
    {
        cout << "Error: opening .raw file failed" << endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << "OK: open .raw file successed" << endl;
    }
    if (fread(data, sizeof(char), size, fp) != size)
    {
        cout << "Error: read .raw file failed" << endl;
        exit(1);
    }
    else
    {
        cout << "OK: read .raw file successed" << endl;
    }
    fclose(fp);

    //cout << "\n The data in head.raw: \n";
    //for (int _i = 0; _i < w * h * d; ++_i)
    //    cout << (int)data[_i] << " ";
    //cout << "Print Finished ..." << endl;

    GLuint g_volTexObj;
    glGenTextures(1, &g_volTexObj);
    // bind 3D texture target
    glBindTexture(GL_TEXTURE_3D, g_volTexObj);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    // pixel transfer happens here from client to OpenGL server
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // glTexImage3D(GL_TEXTURE_3D, 0, GL_INTENSITY, w, h, d, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R16F, w, h, d, 0, GL_RED, GL_UNSIGNED_BYTE, data);
    delete[]data;
    cout << "volume texture created" << endl;
    return g_volTexObj;
}

GLuint initVol3DTexFromGeneratedData() {
    Volume<GLubyte> myvolume;
    vector<GLubyte> data = myvolume.getData();
    glm::vec3 voxsize = myvolume.getVoxelSize();
    GLuint g_volTexObj;
    glGenTextures(1, &g_volTexObj);
    // bind 3D texture target
    glBindTexture(GL_TEXTURE_3D, g_volTexObj);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    // pixel transfer happens here from client to OpenGL server
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // glTexImage3D(GL_TEXTURE_3D, 0, GL_INTENSITY, w, h, d, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R16F, voxsize.x, voxsize.y, voxsize.z, 0, GL_RED, GL_UNSIGNED_BYTE, &data[0]);
    data.clear();
    cout << "volume texture created" << endl;
    return g_volTexObj;
}

void initFrameBuffer(GLuint texObj, GLuint texWidth, GLuint texHeight)
{
    // create a depth buffer for our framebuffer
    GLuint depthBuffer;
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, texWidth, texHeight);

    // attach the texture and the depth buffer to the framebuffer
    glGenFramebuffers(1, &g_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, g_frameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texObj, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
    checkFramebufferStatus();
    glEnable(GL_DEPTH_TEST);
}

void checkFramebufferStatus()
{
    GLenum complete = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    // cout << "I'm in checkFramebufferStatus() \n";
    if (complete != GL_FRAMEBUFFER_COMPLETE)
    {
        cout << "framebuffer is not complete" << endl;
        exit(EXIT_FAILURE);
    }
}

int draw_shape(int shape_flag) {
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

    if (shape_flag == DRAW_CUBIC)
        generateCubicData(Vertices, Indices);
    else if (shape_flag == DRAW_SPHERE)
        generateSphereData(Vertices, Indices);
    else
        throw "No implementations yet!";
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

int VolumeRendering(int volume_choice) {

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
    //Shader lightingShader("myShader/basic_lightning.vs", "myShader/basic_lightning.fs");
    //Shader lightCubeShader("myShader/cubic_lightning.vs", "myShader/volume_basic.fs");

    Shader volumeShader("myShader/volume_raycasting.vs", "myShader/volume_raycasting.fs");
    Shader backShader("myShader/volume_backface.vs", "myShader/volume_backface.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    vector<float> Vertices;
    vector<int> Indices;

    generateCubic_for_vr(Vertices, Indices);
    //generateSphereData(Vertices, Indices);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);


    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    //unsigned int lightCubeVAO;
    //glGenVertexArrays(1, &lightCubeVAO);
    //glBindVertexArray(lightCubeVAO);

    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //// note that we update the lamp's position attribute's stride to reflect the updated buffer data
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    //texture
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    // cout << "===== " << width << " " << height << endl;
    GLuint g_tffTexObj = initTFF1DTex("used_data/tff.dat");
    GLuint g_bfTexObj = initFace2DTex(width, height);
    GLuint g_volTexObj;
    if (volume_choice == HEAD_RENDERING)
        g_volTexObj = initVol3DTex("used_data/head256.raw", 256, 256, 225);
    else if (volume_choice == GEN_RENDERING)
        g_volTexObj = initVol3DTexFromGeneratedData();
    else
        throw "Not Implemented!";
    initFrameBuffer(g_bfTexObj, width, height);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, g_frameBuffer);

    //Volume myVolume;
    //vector<int> data = myVolume.getData();
    //GLuint g_volTexObj;
    //glGenTextures(1, &g_volTexObj);
    //glBindTexture(GL_TEXTURE_3D, g_volTexObj);
    //glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    //glm::vec3 voxelsize = myVolume.getVoxelSize();
    ////glTexImage3D(GL_TEXTURE_3D, 0, GL_INTENSITY, w, h, d, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE,data);
    //glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, voxelsize.x, voxelsize.y, voxelsize.z, 0, GL_RED, GL_INT, &data[0]);
    //cout << "volume texture created!" << endl;

    // volumeShader.setInt("VolumeTex", 2);
    // initFrameBuffer(g_bfTexObj, width, height);
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

        // display
        glEnable(GL_DEPTH_TEST);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, g_frameBuffer);
        glViewport(0, 0, width, height);

        backShader.use();

        // render
        // ------
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //volumeShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        //volumeShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        //volumeShader.setVec3("lightPos", lightPos);
        //volumeShader.setVec3("viewPos", camera.Position);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 model = glm::mat4(1.0f);
        //glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f),
        //    glm::vec3(0.0f, 0.0f, 0.0f),
        //    glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        //model = glm::translate(model, glm::vec3(-0.5f, -0.5f, -0.5f));
        backShader.setMat4("projection", projection);
        backShader.setMat4("view", view);
        backShader.setMat4("model", model);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
        glDisable(GL_CULL_FACE);

        glUseProgram(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, width, height);
        volumeShader.use();
        /*glCullFace(GL_BACK);
        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);*/

        volumeShader.setVec2("ScreenSize", glm::vec2((float)width, (float)height));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_1D, g_tffTexObj);
        volumeShader.setInt("TransferFunc", 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, g_bfTexObj);
        volumeShader.setInt("ExitPoints", 1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_3D, g_volTexObj);
        volumeShader.setInt("VolumeTex", 2);

        //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 view = camera.GetViewMatrix();
        volumeShader.setMat4("projection", projection);
        volumeShader.setMat4("view", view);
        //glm::mat4 model = glm::mat4(1.0f);
        volumeShader.setMat4("model", model);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
        glDisable(GL_CULL_FACE);
        glUseProgram(0);
        //backShader.use();
        //backShader.setMat4("projection", projection);
        //backShader.setMat4("view", view);
        //backShader.setMat4("model", model);
        //glBindVertexArray(cubeVAO);
        //glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);



        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    //glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

#endif // !VOLUME_RENDERING_UTILS
