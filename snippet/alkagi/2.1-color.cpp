#include <alkagi/alkagi.h>

int main()
{
    /** @todo Enum */
    if (init() == 0) return -1;

    /** @todo 파일경로 */
    Shader objectShader("/Users/joseonghyeon/dev/alkagi/src/main.vs", "/Users/joseonghyeon/dev/alkagi/src/main.fs");
    Shader lightShader("/Users/joseonghyeon/dev/alkagi/src/light_source.vs", "/Users/joseonghyeon/dev/alkagi/src/light_source.fs");

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
    };

    uint VBO, objectVAO, lightVAO;

    glGenVertexArrays(1, &objectVAO);
    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(objectVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /**
     * @note
     * VBO stays the same; the vertices are the same for the light object which is also a 3D cube.
     * 
     * @note
     * to link it with glVertexAttribPointer, no need to fill it; the VBO's data already contains all we need.
     * we only need to bind to the VBO.
     * it's already bound, but we do it again for educational purposes.
     * 
     * @todo
     * 아래 내용 전부주석처리해도 문제없이 렌더링 되는데?
    */
    // glBindVertexArray(lightVAO);
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(g_mainWindow))
    {
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        double timeValue = glfwGetTime();
        float cosVal = static_cast<float>(cos(timeValue));
        float sinVal = static_cast<float>(sin(timeValue));

        processInput(g_mainWindow);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        objectShader.use();
        objectShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        // objectShader.setVec3("objectColor", abs(cosVal), abs(cosVal), abs(cosVal));
        objectShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
        // objectShader.setVec3("lightColor",  abs(cosVal), abs(cosVal), abs(cosVal));

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT , 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        objectShader.setMat4("projection", projection);
        objectShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        objectShader.setMat4("model", model);

        /** @todo 주석처리해도 잘되는데? */
        // glBindVertexArray(objectVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lightShader.use();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightShader.setMat4("model", model);

        /** @todo 주석처리해도 잘되는데? */
        // glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(g_mainWindow);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &objectVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}