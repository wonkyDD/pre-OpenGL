#ifndef ALKAGI_H
#define ALKAGI_H

#include <thread>
#include <cmath>
#include <stdio.h>
#include <assert.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <alkagi/shader.h>
#include <alkagi/filesystem.h>
#define STB_IMAGE_IMPLEMENTATION
#include <alkagi/stb_image.h>
// #include <alkagi/camera.h>


/**
 * @todo
 * 1. 함수 declare/define 분리?
 * 2. 전역변수는 어디다 관리할 건지?
 * 
 * -> 구조를 잘 잡아야하므로 일단 더 개발하면서 추후수정.
*/
// #include <alkagi/util.h>


#define ASSERT(x) assert(x)
typedef unsigned int uint;


GLFWwindow* g_mainWindow = nullptr;
const uint WINDOW_WIDTH = 800;
const uint WINDOW_HEIGHT = 600;
float mixValue = 0.5f;
float deltaTime = 0.0f;
float lastTime = 0.0f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.3f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float fov = 45.0f;
float sensitivity = 0.1f;
bool firstMouse = true;
float lastX = WINDOW_WIDTH / 2.0;
float lastY = WINDOW_HEIGHT / 2.0;
float yaw = -90.0f;
float pitch = 0.0f;


int init(const char* caption = "Alkagi");
void processInput(GLFWwindow *window);
void errorCallback(int error, const char* description);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void cursorPosCallback(GLFWwindow* window, double xposIn, double yposIn);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);


/**
 * @todo 
 * glfwInit, glfwCreateWindow, gladLoadGLLoader 별로
 * 리턴하는 int값을 따로 만들어줄지? 아니면 enum을 따로 리턴해줄지?
 * 
 * @todo Enum 네이밍 방식? (EInit은 좀;;)
*/

// enum class EInit 
// {
//     glfwInit,
//     glfwCreateWindow,
//     gladLoadGLLoader
// };


int init(const char* caption)
{
    if (glfwInit() == 0)
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return 0;
	}

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    g_mainWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, caption, NULL, NULL);
    if (g_mainWindow == NULL) 
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(g_mainWindow);
    glfwSetErrorCallback(errorCallback);
    glfwSetFramebufferSizeCallback(g_mainWindow, framebufferSizeCallback);
    glfwSetScrollCallback(g_mainWindow, scrollCallback);
    glfwSetCursorPosCallback(g_mainWindow, cursorPosCallback);

    glfwSetInputMode(g_mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        printf("Failed to initialize GLAD\n");
        return 0;
    }

    return 1;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixValue += 0.005f;
        if (mixValue >= 1.0f) mixValue = 1.0f;
    }
    
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixValue -= 0.005f;
        if (mixValue <= 0.0f) mixValue = 0.0f;
    }

    // float cameraSpeed = 10.0 * deltaTime;
    float cameraSpeed = static_cast<float>(10.0 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void errorCallback(int error, const char* description)
{
	fprintf(stderr, "GLFW error occured. Code: %d. Description: %s\n", error, description);
}

void cursorPosCallback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    
    /** @todo sensitivity란 */
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    /** @todo yaw, pitch, roll이란 */
    yaw += xoffset;
    pitch += yoffset;

    /** @todo 왜 하필 pitch만, 그것도 89.0f란 경계인건지 */
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
        
    /** @todo 뜬금없는 수식들의 근원은? */
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    /**
     * @note
     * 1. scroll을 올림 (offset 양수) 
     * -> 가까워지기를 기대하므로,
     * fov를 낮춰야함
     * 
     * 2. scroll을 내림 (offset 음수)
     * -> 멀어지기를 기대하므로,
     * fov를 높여야함
    */
    fov -= (float)yoffset;
    if (fov > 45.0f) fov = 45.0f;
    if (fov < 1.0f) fov = 1.0f;
}


#endif // ALKAGI_H