// Code for INFORMATION VISUALIZATION TECHNOLOGY
// written by Tiankai Hang (tkhang@seu.edu.cn)
// some code was borrowed from https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/
// 2020/10/06

// function: Draw Cylinder based on GLFW and GLAD

#include "volumerendering_utils.h"


// ====================================================================
//               \\       main function       //
// ====================================================================
int main()
{
    // int flag = draw_shape(DRAW_SPHERE);
    // int flag = draw_shape(DRAW_CUBIC);
    // int flag = VolumeRendering(HEAD_RENDERING);
    int flag = VolumeRendering(GEN_RENDERING);
    return flag;
}
// ====================================================================
// ====================================================================


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

