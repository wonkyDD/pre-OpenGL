#include <opengl/opengl.h>

int main()
{
    /** @todo Enum */
    if (init() == 0) return -1;

    /** @todo 파일경로 */
    Shader mainShader(FileSystem::getPath("src/shader/main.vs").c_str(), FileSystem::getPath("src/shader/main.fs").c_str());
    Shader lightShader(FileSystem::getPath("src/shader/light_source.vs").c_str(), FileSystem::getPath("src/shader/light_source.fs").c_str());

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    unsigned int VBO, mainVAO, lightVAO;

    glGenVertexArrays(1, &mainVAO);
    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(mainVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    /**
     * @note
     * VBO stays the same; the vertices are the same for the light object which is also a 3D cube.
     * To link it with glVertexAttribPointer, no need to fill it; 
     * the VBO's data already contains all we need. We only need to bind to the VBO.
     * it's already bound, but we do it again for educational purposes.
     * 
     * @todo
     * 아래 내용 전부주석처리해도 문제없이 렌더링 되는데?
    */
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int diffuseMap = loadTexture(FileSystem::getPath("resources/textures/container2.png").c_str());
    unsigned int specularMap = loadTexture(FileSystem::getPath("resources/textures/container2_specular.png").c_str());

    /** @todo 파일경로  */
    mainShader.use();
    mainShader.setInt("material.diffuse", 0);
    mainShader.setInt("material.specular", 1);

    while (!glfwWindowShouldClose(g_mainWindow))
    {
        float currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        /** @todo glm::cos,sin 으로 대체할 것 */
        // double timeValue = currentTime * 3.0;
        // float cosVal = static_cast<float>(cos(timeValue));
        // float sinVal = static_cast<float>(sin(timeValue));

        processInput(g_mainWindow);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mainShader.use();
        mainShader.setVec3("viewPos", camera.Position);
        mainShader.setFloat("material.shininess", 32.0f);

        // DirLight
        mainShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        mainShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        mainShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        mainShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

        // PointLight
        mainShader.setVec3("pointLight[0].position", pointLightPositions[0]);
        mainShader.setVec3("pointLight[0].ambient", 0.05f, 0.05f, 0.05f);
        mainShader.setVec3("pointLight[0].diffuse", 0.8f, 0.8f, 0.8f);
        mainShader.setVec3("pointLight[0].specular", 1.0f, 1.0f, 1.0f);
        mainShader.setFloat("pointLight[0].constant", 1.0f);
        mainShader.setFloat("pointLight[0].linear", 0.09f);
        mainShader.setFloat("pointLight[0].quadratic", 0.032f);
        
        mainShader.setVec3("pointLight[1].position", pointLightPositions[1]);
        mainShader.setVec3("pointLight[1].ambient", 0.05f, 0.05f, 0.05f);
        mainShader.setVec3("pointLight[1].diffuse", 0.8f, 0.8f, 0.8f);
        mainShader.setVec3("pointLight[1].specular", 1.0f, 1.0f, 1.0f);
        mainShader.setFloat("pointLight[1].constant", 1.0f);
        mainShader.setFloat("pointLight[1].linear", 0.09f);
        mainShader.setFloat("pointLight[1].quadratic", 0.032f);
        
        mainShader.setVec3("pointLight[2].position", pointLightPositions[2]);
        mainShader.setVec3("pointLight[2].ambient", 0.05f, 0.05f, 0.05f);
        mainShader.setVec3("pointLight[2].diffuse", 0.8f, 0.8f, 0.8f);
        mainShader.setVec3("pointLight[2].specular", 1.0f, 1.0f, 1.0f);
        mainShader.setFloat("pointLight[2].constant", 1.0f);
        mainShader.setFloat("pointLight[2].linear", 0.09f);
        mainShader.setFloat("pointLight[2].quadratic", 0.032f);
        
        mainShader.setVec3("pointLight[3].position", pointLightPositions[3]);
        mainShader.setVec3("pointLight[3].ambient", 0.05f, 0.05f, 0.05f);
        mainShader.setVec3("pointLight[3].diffuse", 0.8f, 0.8f, 0.8f);
        mainShader.setVec3("pointLight[3].specular", 1.0f, 1.0f, 1.0f);
        mainShader.setFloat("pointLight[3].constant", 1.0f);
        mainShader.setFloat("pointLight[3].linear", 0.09f);
        mainShader.setFloat("pointLight[3].quadratic", 0.032f);

        // SpotLight
        mainShader.setVec3("spotLight.position", camera.Position);
        mainShader.setVec3("spotLight.direction", camera.Front);
        mainShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        mainShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        mainShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        mainShader.setFloat("spotLight.constant", 1.0f);
        mainShader.setFloat("spotLight.linear", 0.09f);
        mainShader.setFloat("spotLight.quadratic", 0.032f);
        mainShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        mainShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT , 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        mainShader.setMat4("projection", projection);
        mainShader.setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        mainShader.setMat4("model", model);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);


        /** @todo 주석처리해도 잘되는데? */
        glBindVertexArray(mainVAO);
        for (unsigned int i = 0; i < 10; ++i)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            mainShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        lightShader.use();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);
        /** @todo 주석처리해도 잘되는데? */
        glBindVertexArray(lightVAO);
        for (unsigned int i = 0; i < 4; ++i)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lightShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(g_mainWindow);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &mainVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}