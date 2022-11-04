#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <cmath>

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
    glViewport(0, 0, width, height);
}

// global
GLFWwindow* gMainWindow = nullptr;
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

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
    gMainWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Alkagi", NULL, NULL);
    if (gMainWindow == NULL) 
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(gMainWindow);

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
    */
    glfwSetFramebufferSizeCallback(gMainWindow, framebufferSizeCallback);
    // glfwSetWindowSizeCallback(gMainWindow, ResizeWindowCallback);
	// glfwSetKeyCallback(gMainWindow, KeyCallback);
	// glfwSetCharCallback(gMainWindow, CharCallback);
	// glfwSetMouseButtonCallback(gMainWindow, MouseButtonCallback);
	// glfwSetCursorPosCallback(gMainWindow, MouseMotionCallback);
	// glfwSetScrollCallback(gMainWindow, ScrollCallback);

    int success;
    char infoLog[512];

    // vertex shader
    // https://registry.khronos.org/OpenGL-Refpages/gl4/
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
    }

    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
    }

    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) 
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s", infoLog);
    }

    // delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = 
    {
        0.5f, -0.5f, 0.0f,      // bottom right
        -0.5f, -0.5f, 0.0f,     // bottom left
        0.0f, 0.5f, 0.0f,       // top 
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // bind vertex array
    glBindVertexArray(VAO);

    // bind vertex butter, set buffer data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // configure vertex attribute
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
    while (!glfwWindowShouldClose(gMainWindow))
    {
        /**
         * @todo
         * callback으로 루프 들어가기 전에 처리?
        */
        processInput(gMainWindow);
        
        /**
         * @todo
         * glClearColor vs glDrawArrays
        */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // activate the shader before any calls to glUniform
        glUseProgram(shaderProgram);

        double  timeValue = glfwGetTime();
        float greenValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);

        /**
         * @todo
         * shader_program에 attach된 모든 shader안 변수들은
         * glGet...을 통해 접근/수정 할수있나?
         * 
         * triangle 회전시키기?
        */
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        // render the triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /**
         * @todo
         * swap interval
        */
        glfwSwapBuffers(gMainWindow);
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
