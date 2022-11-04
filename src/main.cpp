#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <cmath>

void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// whenever the window size changed (by OS or userss)
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    /**
     * @todo
     * Make sure the viewport matches the new window dimensions.
     * On retina displays, w and h will be significantly larger than specified.
    */
    glViewport(0, 0, width, height);
}

// global
GLFWwindow* g_mainWindow = nullptr;
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

const char* vertex_shader_source ="#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "}\0";

const char* fragment_shader_source = "#version 330 core\n"
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
    */
    glfwSetFramebufferSizeCallback(g_mainWindow, framebuffer_size_callback);
    // glfwSetWindowSizeCallback(g_mainWindow, ResizeWindowCallback);
	// glfwSetKeyCallback(g_mainWindow, KeyCallback);
	// glfwSetCharCallback(g_mainWindow, CharCallback);
	// glfwSetMouseButtonCallback(g_mainWindow, MouseButtonCallback);
	// glfwSetCursorPosCallback(g_mainWindow, MouseMotionCallback);
	// glfwSetScrollCallback(g_mainWindow, ScrollCallback);

    int success;
    char infoLog[512];

    // vertex shader
    // https://registry.khronos.org/OpenGL-Refpages/gl4/
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
    }

    // fragment shader
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
    }

    // link shaders
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) 
    {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s", infoLog);
    }

    // delete shaders
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

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
    while (!glfwWindowShouldClose(g_mainWindow))
    {
        /**
         * @todo
         * callback으로 루프 들어가기 전에 처리?
        */
        process_input(g_mainWindow);
        
        /**
         * @todo
         * glClearColor vs glDrawArrays
        */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // activate the shader before any calls to glUniform
        glUseProgram(shader_program);

        double  time_value = glfwGetTime();
        float green_value = static_cast<float>(sin(time_value) / 2.0 + 0.5);

        /**
         * @todo
         * shader_program에 attach된 모든 shader안 변수들은
         * glGet...을 통해 접근/수정 할수
        */
        int vertex_color_location = glGetUniformLocation(shader_program, "ourColor");
        glUniform4f(vertex_color_location, 0.0f, 0.0f, green_value, 1.0f);

        // render the triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // swap buffers, poll IO events
        glfwSwapBuffers(g_mainWindow);
        glfwPollEvents();
    }

    // delete vertex array, buffer, program
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader_program);

    // clear allocated GLFW resources
    glfwTerminate();
    return 0;
}
