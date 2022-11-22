#ifndef ALKAGI_H
#define ALKAGI_H

#include <thread>
#include <cmath>
#include <cstdlib>
#include <time.h>
#include <stdio.h>
#include <assert.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <opengl/shader.h>
#include <opengl/filesystem.h>
#include <opengl/camera.h>


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

float deltaTime = 0.0f;
float lastTime = 0.0f;

bool firstMouse = true;
float lastX = WINDOW_WIDTH / 2.0;
float lastY = WINDOW_HEIGHT / 2.0;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
// float strength = 0.1f;


int init(const char* caption = "OpenGL");
void processInput(GLFWwindow* window);
void cursorPosCallback(GLFWwindow* window, double xposIn, double yposIn);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void errorCallback(int error, const char* description);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);


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

    /** @todo 파일경로 */
    /** @todo shader 컴파일도 Init에서 처리하는 방법? */
    // Shader mainShader("/Users/joseonghyeon/dev/alkagi/src/main.vs", "/Users/joseonghyeon/dev/alkagi/src/main.fs");
    // Shader lightShader("/Users/joseonghyeon/dev/alkagi/src/light_source.vs", "/Users/joseonghyeon/dev/alkagi/src/light_source.fs");

    return 1;
}

uint loadTexture(const char *path)
{
    uint textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)      format = GL_RED;
        else if (nrComponents == 3) format = GL_RGB;
        else if (nrComponents == 4) format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        fprintf(stderr, "Texture failed to load at path: %s\n", path);
        stbi_image_free(data);
    }

    return textureID;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
    
    /**
     * @todo
     * ImGui로 ambient, specular의
     * intensity를 각각 조절하도록
    */
    // if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    // {
    //     strength += 0.005f;
    //     if (strength >= 1.0f) strength = 1.0f;
    // }
    
    // if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    // {
    //     strength -= 0.005f;
    //     if (strength <= 0.0f) strength = 0.0f;
    // }

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