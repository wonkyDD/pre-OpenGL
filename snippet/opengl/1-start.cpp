#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <cmath>

typedef unsigned int uint;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// global
GLFWwindow* g_mainWindow = nullptr;
const uint WINDOW_WIDTH = 800;
const uint WINDOW_HEIGHT = 600;

const char* vertexShaderSource ="#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n"
    "}\n\0";

int main()
{
    // setting (preset)


    // init 
    if (glfwInit() == 0)
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

    // hint
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // create window
    g_mainWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Alkagi", NULL, NULL);
    if (g_mainWindow == NULL) 
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(g_mainWindow);

    /**
     * @todo
     * load OpenGL functions (function pointers) 
    */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        printf("Failed to initialize GLAD\n");
        return -1;
    }
    
    /**
     * @todo
     * callback
     * 
     * glfwSetWindowSizeCallback(g_mainWindow, ResizeWindowCallback);
     * glfwSetKeyCallback(g_mainWindow, KeyCallback);
     * glfwSetCharCallback(g_mainWindow, CharCallback);
     * glfwSetMouseButtonCallback(g_mainWindow, MouseButtonCallback);
     * glfwSetCursorPosCallback(g_mainWindow, MouseMotionCallback);
     * glfwSetScrollCallback(g_mainWindow, ScrollCallback);
    */
    glfwSetFramebufferSizeCallback(g_mainWindow, framebufferSizeCallback);
    
    // shader
    int success;
    char infoLog[512];

    uint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
    }

    uint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
    }

    uint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) 
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s", infoLog);
    }

    /**
     * @todo
     * delete shader 타이밍?
    */
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = 
    {
        0.5f, -0.5f, 0.0f,      // bottom right
        -0.5f, -0.5f, 0.0f,     // bottom left
        0.0f, 0.5f, 0.0f,       // top 
    };

    uint VBO, VAO;
    /**
     * @todo
     * glGenVertexArrays(8, &VAO);
     * glGenVertexArrays(10, &VAO);
     * glGenBuffers(2, &VBO);
    */
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /**
     * @todo
     * -> index바꾸기, size바꾸기, pointer바꾸기
     * 
     * glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
     * glVertexAttribPointer(0, i<=0 or i>4, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
     * glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)12);
     * 
     * glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)12);
     * glEnableVertexAttribArray(1);
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /**
     * @todo
     * 뭔 소리임 도대체?
     * 
     * You can unbind the VAO afterwards,
     * so other VAO calls won't accidentally modify this VAO.
     * But this rarely happens.
     * 
     * Modifying other VAOs requires a call to glBindVertexArray anyways
     * so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    */
    // glBindVertexArray(0);

    /**
     * @todo
     * 위에서 이미 해줬는데 왜 또함?
     * 
     * -> It was already bound, but just to demonstrate.
     * 
     * -> This is another approach. 
     * Seeing as we only have a single VAO, 
     * we can just bind it beforehand 
     * before rendering the respective triangle.
    */
    glBindVertexArray(VAO);

    // loop
    while (!glfwWindowShouldClose(g_mainWindow))
    {
        /**
         * @todo
         * callback으로 루프 들어가기 전에 처리?
        */
        processInput(g_mainWindow);
        
        /**
         * @todo
         * glClearColor vs glDrawArrays vs glDrawBuffer
        */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /**
         * @todo
         * activate the shader before any calls to glUniform
         * -> 루프안에다가 넣는이유?
        */
        glUseProgram(shaderProgram);

        double timeValue = glfwGetTime() * 3;
        float greenValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);

        /**
         * @todo
         * triangle 회전시키기?
        */
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /**
         * @todo
         * swap interval
        */
        glfwSwapBuffers(g_mainWindow);
        glfwPollEvents();
    }

    // delete vertex array, buffer, program
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // clear allocated GLFW resources
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
    /**
     * @todo
     * Make sure the viewport matches the new window dimensions.
     * On retina displays, w and h will be significantly larger than specified.
    */

   /**
    * @todo
    * glfwGetWindowSize와
    * glfwGetFramebufferSize로 로그체크하니까
    * callback없이도 width, height 잘바뀌던데 무슨의미임?
   */
    glViewport(0, 0, width, height);
}
