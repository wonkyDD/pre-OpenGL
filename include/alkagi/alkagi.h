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
#include <alkagi/camera.h>


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
bool firstMouse = true;
float lastX = WINDOW_WIDTH / 2.0;
float lastY = WINDOW_HEIGHT / 2.0;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));


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

    /** @todo InputMode */
    glfwSetInputMode(g_mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glfwSetInputMode(g_mainWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        printf("Failed to initialize GLAD\n");
        return 0;
    }

    /** @todo 위치 */
    glEnable(GL_DEPTH_TEST);

    return 1;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
    
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

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);
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

    camera.ProcessMouseMovement(xoffset, yoffset, GL_TRUE);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void errorCallback(int error, const char* description)
{
	fprintf(stderr, "GLFW error occured. Code: %d. Description: %s\n", error, description);
}

#endif // ALKAGI_H