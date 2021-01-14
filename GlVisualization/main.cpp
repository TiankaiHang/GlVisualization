// Code for INFORMATION VISUALIZATION TECHNOLOGY
// written by Tiankai Hang (tkhang@seu.edu.cn)
// some code was borrowed from https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/
// 2020/10/06

// function: Draw Cylinder based on GLFW and GLAD

#include "volumerendering_utils.h"


int VolumeRendering();

// ====================================================================
//               \\       main function       //
// ====================================================================
int main()
{
    // int flag = draw_shape(DRAW_SPHERE);
    int flag = VolumeRendering();
    // int flag = initVol3DTex("used_data/head256.raw", 256, 256, 225);
    return flag;
}
// ====================================================================
// ====================================================================

int VolumeRendering() {

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
    GLuint g_volTexObj = initVol3DTex("used_data/head256.raw", 256, 256, 225);
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
        glm::mat4 view = camera.GetViewMatrix();
        backShader.setMat4("projection", projection);
        backShader.setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        backShader.setMat4("model", model);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
        glDisable(GL_CULL_FACE);

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

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
        glDisable(GL_CULL_FACE);

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

