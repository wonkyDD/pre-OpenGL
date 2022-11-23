#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <alkagi/shader.h>
#include <alkagi/filesystem.h>
#include <stdio.h>

typedef unsigned int uint;

void glfwErrorCallback(int error, const char* description);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

GLFWwindow* g_mainWindow = nullptr;
const uint WINDOW_WIDTH = 800;
const uint WINDOW_HEIGHT = 600;

int main()
{
    if (glfwInit() == 0)
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    g_mainWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Alkagi", NULL, NULL);
    if (g_mainWindow == NULL) 
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(g_mainWindow);
    glfwSetErrorCallback(glfwErrorCallback);
    glfwSetFramebufferSizeCallback(g_mainWindow, framebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    /**
     * @todo
     * root 하드코딩 수정
    */
    Shader ourShader("/Users/joseonghyeon/dev/alkagi/src/main.vs", "/Users/joseonghyeon/dev/alkagi/src/main.fs");
    Shader ourShader2("/Users/joseonghyeon/dev/alkagi/src/main.vs", "/Users/joseonghyeon/dev/alkagi/src/main2.fs");

    /**
     * @note
     * 직사각형 
     * 
     * -> viewport기준 정사각형임.
     * Normalized Device Coordinates와 
     * Screen Space Coordinates 참조
    */
    // float vertices[] = {
    //     // positions         // colors
    //     -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // top left
    //      0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // top right
    //      0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
    //     -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f   // bottom left
    // };

    // uint indices[] = {
    //     0, 1, 3,
    //     1, 2, 3
    // };


    /**
     * @note
     * 마름모
    */
    // float vertices[] = {
    //     // positions         // colors
    //      0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // top
    //     -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  // left
    //      0.0f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom
    //      0.5f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f   // right
    // };

    // uint indices[] = {
    //     0, 1, 3,
    //     1, 2, 3
    // };


    /**
     * @note
     * 평행사변형
    */
    // float vertices[] = {
    //     // positions         // colors
    //      0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // top left
    //     -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // bottom left
    //      0.0f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
    //      0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f   // top right
    // };

    // uint indices[] = {
    //     0, 1, 3,
    //     1, 2, 3
    // };


    /**
     * @note
     * 정육각형
    */
    // float vertices[] = {
    //     // positions           // colors
    //     -0.25f, 0.5f,  0.0f,   0.0f, 0.0f, 1.0f,  // top left
    //     -0.5f,  0.0f,  0.0f,   0.0f, 0.0f, 1.0f,  // middle left
    //     -0.25f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // bottom left
    //      0.25f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // bottom right
    //      0.5f,  0.0f,  0.0f,   0.0f, 1.0f, 0.0f,  // middle right
    //      0.25f, 0.5f,  0.0f,   0.0f, 1.0f, 0.0f,  // top right
    // };

    // uint indices[] = {
    //     0, 1, 2,
    //     0, 2, 3,
    //     0, 3, 4,
    //     0, 4, 5
    // };


    /**
     * @note
     * 직각삼각형 2개
    */
    // float vertices[] = {
    //     // positions         // colors
    //     -0.25f, 0.25f,  0.0f,  0.0f, 0.0f, 1.0f,  // top
    //     -0.25f, -0.25f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
    //     0.25f,  -0.25f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
    //     0.25f,   0.25f,  0.0f,  0.0f, 0.0f, 1.0f,  // top
    //     0.25f, -0.25f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
    //     0.75f,  -0.25f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
    // };


    float vertices[] = {
        // positions         // colors
        -0.25f, 0.25f,  0.0f,  0.0f, 0.0f, 1.0f,  // top
        -0.25f, -0.25f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
        0.25f,  -0.25f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
    };

    float vertices2[] = {
        0.3f,   0.25f,  0.0f,  0.0f, 0.0f, 1.0f,  // top
        0.3f, -0.25f, 0.0f,  0.0f, 1.0f, 0.0f,    // bottom left
        0.8f,  -0.25f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    };

    // uint indices[] = {
    //     0, 1, 3,
    //     1, 2, 3
    // };

    // uint EBO;
    // glGenBuffers(1, &EBO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    
    uint VBO, VAO;
    uint VBO2, VAO2;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    while (!glfwWindowShouldClose(g_mainWindow))
    {
        processInput(g_mainWindow);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


        ourShader.use();
        // ourShader2.use();
        glBindVertexArray(VAO);

        /**
         * @todo
         * 1. shader program 2개사용
         * 
         * 2. program은 각각 다른 fs사용
         * (기존 main.fs와 맨처음 fs를 main2.fs에 넣을 것)
         * 
         * 3. vao, vbo 2개씩 생성
         * 
         * 4. 각 vao, vbo 쌍으로 첫번째는 평범한 직각삼각형,
         * 두번째는 맨처음 greenValue를 uniform으로 넣어주는 직각삼각형 처리
        */

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(g_mainWindow);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void glfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "GLFW error occured. Code: %d. Description: %s\n", error, description);
}
