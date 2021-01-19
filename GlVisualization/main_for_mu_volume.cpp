//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <iostream>
//#include <vector>
//#include <map>
//#include <string>
//#include <cmath>
//#include <fstream>
//// glm
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include "shader.h"
//#include "camera.h"
//
//using namespace std;
//
//
//// settings
//const unsigned int SCREEN_WIDTH = 600;
//const unsigned int SCREEN_HEIGHT = 600;
//
//// camera
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//float lastX = SCREEN_WIDTH / 2.0f;
//float lastY = SCREEN_HEIGHT / 2.0f;
//bool firstMouse = true;
//
//// timing
//float deltaTime = 0.0f;
//float lastFrame = 0.0f;
//
//// lighting
//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
//
//GLuint g_frameBuffer;
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    // make sure the viewport matches the new window dimensions; note that width and 
//    // height will be significantly larger than specified on retina displays.
//    glViewport(0, 0, width, height);
//}
//
//void mouse_callback(GLFWwindow* window, double xpos, double ypos)
//{
//    if (firstMouse)
//    {
//        lastX = xpos;
//        lastY = ypos;
//        firstMouse = false;
//    }
//
//    float xoffset = xpos - lastX;
//    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
//
//    lastX = xpos;
//    lastY = ypos;
//
//    camera.ProcessMouseMovement(xoffset, yoffset);
//}
//
//// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
//// ---------------------------------------------------------------------------------------------------------
//void processInput(GLFWwindow* window)
//{
//    // exit
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//
//    // w, s, d, a: to change the view point
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
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//    camera.ProcessMouseScroll(yoffset);
//}
//
//GLuint initTFF1DTex(const char* filename)
//{
//    // read in the user defined data of transfer function
//    ifstream inFile(filename, ifstream::in);
//    if (!inFile)
//    {
//        cerr << "Error openning file: " << filename << endl;
//        exit(EXIT_FAILURE);
//    }
//
//    const int MAX_CNT = 10000;
//    GLubyte* tff = (GLubyte*)calloc(MAX_CNT, sizeof(GLubyte));
//    inFile.read(reinterpret_cast<char*>(tff), MAX_CNT);
//    size_t bytecnt;
//    if (inFile.eof())
//    {
//        bytecnt = inFile.gcount();
//        *(tff + bytecnt) = '\0';
//        cout << "bytecnt " << bytecnt << endl;
//    }
//    else if (inFile.fail())
//    {
//        cout << filename << "read failed " << endl;
//    }
//    else
//    {
//        cout << filename << "is too large" << endl;
//    }
//
//    //for (size_t i = 0; i < bytecnt; ++i) {
//    //    tff[i] = (GLubyte)128;
//    //}
//
//    GLuint tff1DTex;
//    glGenTextures(1, &tff1DTex);
//    glBindTexture(GL_TEXTURE_1D, tff1DTex);
//    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, tff);
//
//    // print tff
//    /*for (int i = 0; i < MAX_CNT; ++i) {
//        cout << (int)tff[i] << " ";
//    }*/
//    free(tff);
//    return tff1DTex;
//}
//
//GLuint initFace2DTex(GLuint bfTexWidth, GLuint bfTexHeight)
//{
//    GLuint backFace2DTex;
//    glGenTextures(1, &backFace2DTex);
//    glBindTexture(GL_TEXTURE_2D, backFace2DTex);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, bfTexWidth, bfTexHeight, 0, GL_RGBA, GL_FLOAT, NULL);
//    return backFace2DTex;
//}
//
//GLuint initVol3DTex(const char* filename, GLuint w, GLuint h, GLuint d)
//{
//
//    FILE* fp;
//    size_t size = w * h * d;
//    GLubyte* data = new GLubyte[size];			  // 8bit
//    if (!(fp = fopen(filename, "rb"))){
//        cout << "Error: opening .raw file failed" << endl;
//        exit(EXIT_FAILURE);
//    }
//    else{
//        cout << "OK: open .raw file successed" << endl;
//    }
//    if (fread(data, sizeof(char), size, fp) != size){
//        cout << "Error: read .raw file failed" << endl;
//        exit(1);
//    }
//    else{
//        cout << "OK: read .raw file successed" << endl;
//    }
//    fclose(fp);
//
//    GLuint g_volTexObj;
//    glGenTextures(1, &g_volTexObj);
//    // bind 3D texture target
//    glBindTexture(GL_TEXTURE_3D, g_volTexObj);
//    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
//    // pixel transfer happens here from client to OpenGL server
//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//    // glTexImage3D(GL_TEXTURE_3D, 0, GL_INTENSITY, w, h, d, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
//    glTexImage3D(GL_TEXTURE_3D, 0, GL_R16F, w, h, d, 0, GL_RED, GL_UNSIGNED_BYTE, data);
//    delete[]data;
//    cout << "volume texture created" << endl;
//    return g_volTexObj;
//}
//
//template<typename Dtype>
//bool inBall(Dtype center_x, Dtype center_y, Dtype center_z, Dtype radius,
//    Dtype x, Dtype y, Dtype z) {
//    return (x - center_x) * (x - center_x) + (y - center_y) * (y - center_y) +
//        (z - center_z) * (z - center_z) < radius * radius;
//}
//
//template<typename Dtype>
//bool inCylinder(Dtype center_x, Dtype center_y, Dtype center_z, Dtype radius,
//    Dtype length, Dtype x, Dtype y, Dtype z) {
//
//    return ((x - center_x) * (x - center_x) + (y - center_y) * (y - center_y) <=
//        radius * radius) && (abs(z - center_z) <= length / 2);
//}
//
//void generateVolumeData(vector<GLubyte>& data, int w, int h, int d) {
//    int size_of_volume = w * h * d;
//    data.resize(size_of_volume);
//    glm::ivec3 center(w / 2, h / 2, h / 2);
//    int radius_ball = 100;
//    int radius_cylinder = 50;
//    int length_cylinder = 2 * int(sqrtf(radius_ball * radius_ball - radius_cylinder * radius_cylinder));
//
//    for (int i = 0; i < w; ++i) {
//        for (int j = 0; j < h; ++j) {
//            for (int k = 0; k < h; ++k) {
//                bool isInBall = inBall(center.x, center.y, center.z, radius_ball, i, j, k);
//                bool isInCylinder = inCylinder(center.x, center.y, center.z,
//                    radius_cylinder, length_cylinder, i, j, k);
//                if (isInBall)
//                    data[i * d * w + j * d + k] = (GLubyte)100;
//                if (isInCylinder)
//                    data[i * d * w + j * d + k] = (GLubyte)200;
//            }
//        }
//    }
//}
//
//GLuint initVol3DTexFromGeneratedData() {
//    vector<GLubyte> data;
//    generateVolumeData(data, 256, 256, 256);
//    glm::vec3 voxsize(256, 256, 256);
//    GLuint g_volTexObj;
//    glGenTextures(1, &g_volTexObj);
//    // bind 3D texture target
//    glBindTexture(GL_TEXTURE_3D, g_volTexObj);
//    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
//    // pixel transfer happens here from client to OpenGL server
//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//    // glTexImage3D(GL_TEXTURE_3D, 0, GL_INTENSITY, w, h, d, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
//    glTexImage3D(GL_TEXTURE_3D, 0, GL_R16F, voxsize.x, voxsize.y, voxsize.z, 0, GL_RED, GL_UNSIGNED_BYTE, &data[0]);
//    data.clear();
//    cout << "volume texture created" << endl;
//    return g_volTexObj;
//}
//
//void checkFramebufferStatus()
//{
//    GLenum complete = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//    // cout << "I'm in checkFramebufferStatus() \n";
//    if (complete != GL_FRAMEBUFFER_COMPLETE)
//    {
//        cout << "framebuffer is not complete" << endl;
//        exit(EXIT_FAILURE);
//    }
//}
//
//void initFrameBuffer(GLuint texObj, GLuint texWidth, GLuint texHeight)
//{
//    // create a depth buffer for our framebuffer
//    GLuint depthBuffer;
//    glGenRenderbuffers(1, &depthBuffer);
//    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, texWidth, texHeight);
//
//    // attach the texture and the depth buffer to the framebuffer
//    glGenFramebuffers(1, &g_frameBuffer);
//    glBindFramebuffer(GL_FRAMEBUFFER, g_frameBuffer);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texObj, 0);
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
//    checkFramebufferStatus();
//    glEnable(GL_DEPTH_TEST);
//}
//
//int main() {
//
//    // glfw: initialize and configure
//    // ------------------------------
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//#ifdef __APPLE__
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif
//
//    // glfw window creation
//    // --------------------
//    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "PROJECT TWO", NULL, NULL);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//    glfwSetCursorPosCallback(window, mouse_callback);
//    glfwSetScrollCallback(window, scroll_callback);
//
//    // tell GLFW to capture our mouse
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
//    // glad: load all OpenGL function pointers
//    // ---------------------------------------
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//
//    // configure global opengl state
//    // -----------------------------
//    glEnable(GL_DEPTH_TEST);
//
//    Shader volumeShader("shader/volume_raycasting.vs", "shader/volume_raycasting.fs");
//    Shader backShader("shader/volume_backface.vs", "shader/volume_backface.fs");
//
//    // set up vertex data (and buffer(s)) and configure vertex attributes
//    // ------------------------------------------------------------------
//
//    GLfloat Vertices[24] = {
//    0.0, 0.0, 0.0,
//    0.0, 0.0, 1.0,
//    0.0, 1.0, 0.0,
//    0.0, 1.0, 1.0,
//    1.0, 0.0, 0.0,
//    1.0, 0.0, 1.0,
//    1.0, 1.0, 0.0,
//    1.0, 1.0, 1.0
//    };
//    GLuint Indices[36] = {
//    1,5,7,
//    7,3,1,
//    0,2,6,
//    6,4,0,
//    0,1,3,
//    3,2,0,
//    7,5,4,
//    4,6,7,
//    2,3,7,
//    7,6,2,
//    1,0,4,
//    4,5,1
//    };
//
//
//    // first, configure the cube's VAO (and VBO)
//    unsigned int VBO, cubeVAO, EBO;
//    glGenVertexArrays(1, &cubeVAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//
//    glBindVertexArray(cubeVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), Vertices, GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(int), Indices, GL_STATIC_DRAW);
//
//    // position attribute
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    // color attribute
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(1);
//
//    //texture
//    int width, height;
//    glfwGetFramebufferSize(window, &width, &height);
//    GLuint g_tffTexObj = initTFF1DTex("used_data/tff.dat");
//    GLuint g_bfTexObj = initFace2DTex(width, height);
//    GLuint g_volTexObj;
//
//    g_volTexObj = initVol3DTexFromGeneratedData();
//
//    initFrameBuffer(g_bfTexObj, width, height);
//    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, g_frameBuffer);
//
//    while (!glfwWindowShouldClose(window))
//    {
//        // per-frame time logic
//        // --------------------
//        float currentFrame = glfwGetTime();
//        deltaTime = currentFrame - lastFrame;
//        lastFrame = currentFrame;
//
//        // input
//        // -----
//        processInput(window);
//
//        // display
//        glEnable(GL_DEPTH_TEST);
//        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, g_frameBuffer);
//        glViewport(0, 0, width, height);
//
//        backShader.use();
//
//        // render
//        // ------
//        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
//        glm::mat4 model = glm::mat4(1.0f);
//        glm::mat4 view = camera.GetViewMatrix();
//
//        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//        //model = glm::translate(model, glm::vec3(-0.5f, -0.5f, -0.5f));
//        backShader.setMat4("projection", projection);
//        backShader.setMat4("view", view);
//        backShader.setMat4("model", model);
//
//        glEnable(GL_CULL_FACE);
//        glCullFace(GL_FRONT);
//        glBindVertexArray(cubeVAO);
//        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
//        glDisable(GL_CULL_FACE);
//
//        glUseProgram(0);
//        glBindFramebuffer(GL_FRAMEBUFFER, 0);
//        glViewport(0, 0, width, height);
//        volumeShader.use();
//
//        volumeShader.setVec2("ScreenSize", glm::vec2((float)width, (float)height));
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_1D, g_tffTexObj);
//        volumeShader.setInt("TransferFunc", 0);
//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_2D, g_bfTexObj);
//        volumeShader.setInt("ExitPoints", 1);
//        glActiveTexture(GL_TEXTURE2);
//        glBindTexture(GL_TEXTURE_3D, g_volTexObj);
//        volumeShader.setInt("VolumeTex", 2);
//
//        volumeShader.setMat4("projection", projection);
//        volumeShader.setMat4("view", view);
//        volumeShader.setMat4("model", model);
//
//        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        glEnable(GL_CULL_FACE);
//        glCullFace(GL_BACK);
//        glBindVertexArray(cubeVAO);
//        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
//        glDisable(GL_CULL_FACE);
//        glUseProgram(0);
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    // optional: de-allocate all resources once they've outlived their purpose:
//    // ------------------------------------------------------------------------
//    glDeleteVertexArrays(1, &cubeVAO);
//    //glDeleteVertexArrays(1, &lightCubeVAO);
//    glDeleteBuffers(1, &VBO);
//    glDeleteBuffers(1, &EBO);
//
//    // glfw: terminate, clearing all previously allocated GLFW resources.
//    // ------------------------------------------------------------------
//    glfwTerminate();
//
//    return 0;
//}