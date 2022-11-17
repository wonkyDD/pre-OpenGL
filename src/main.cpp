#include <alkagi/alkagi.h>

int main()
{
    /** @todo Enum */
    if (init() == 0) return -1;

    glEnable(GL_DEPTH_TEST);

    /** @todo 파일경로 하드코딩 */
    Shader ourShader("/Users/joseonghyeon/dev/alkagi/src/main.vs", "/Users/joseonghyeon/dev/alkagi/src/main.fs");

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
    
    /**
     * @todo
     * 굳이 indexed drawing을 안한거는
     * 오히려 vertex를 줄이려다가
     * index를 써놓는게 가독성을 해칠거라고 생각해서인가?
    */
    // unsigned int indices[] = {  
    //     0, 1, 3, // first triangle
    //     1, 2, 3  // second triangle
    // };

    uint VBO, VAO;
    // uint EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    uint texture1, texture2;
    int width, height, nrChannels;

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /** @todo getPath 하드코딩 */
    stbi_set_flip_vertically_on_load(true); 
    unsigned char* data = stbi_load(FileSystem::getPath("resources/textures/container.jpg").c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        fprintf(stderr, "Failed to load texture\n");
    }
    stbi_image_free(data);


    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /** @todo getPath 하드코딩 */
    data = stbi_load(FileSystem::getPath("resources/textures/github2.png").c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        fprintf(stderr, "Failed to load texture\n");
    }
    stbi_image_free(data);

    ourShader.use();
    ourShader.setInt("texture1", 1);
    ourShader.setInt("texture2", 2);

    while (!glfwWindowShouldClose(g_mainWindow))
    {
        processInput(g_mainWindow);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glClear(GL_COLOR_BUFFER_BIT);
        
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, texture2);

        ourShader.use();
        ourShader.setFloat("mixValue", mixValue);

        double timeValue = glfwGetTime();
        float cosVal = static_cast<float>(cos(timeValue));
        float sinVal = static_cast<float>(sin(timeValue));

        // glm::mat4 model         = glm::mat4(1.0f);
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);

        /**
         * @note
         * we're translating the scene in the reverse direction of where we want to move
        */
        view = glm::rotate(view, (float)timeValue * 5, glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -7.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT , 0.1f, 100.0f);

        // ourShader.setMat4("model", model);
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        // glBindVertexArray(VAO);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(VAO);
        for (int i = 0; i < 10; ++i)
        {
            glm::mat4 model = glm::mat4(1.0f);

            /**
             * @note
             * scale이 translate보다 앞서면 [거리유지x == 회전중심스케일]
             * rotate가 translate보다 앞서면 [궤도회전]
             * 
             * case1,2가 차이없는 이유는
             * 결정적인 translate를 앞에다박아놔서
             * 연산이 제일 마지막에 되도록 해놓은 거라
             * s,r순서를 변경해도 의미가 없다.
             * 
             * 
             * case1, case2     : 이동 -> 자체회전, 자체스케일 
             * case3            : 이동 -> 궤도회전, 자체스케일
             * case4, case6     : 궤도회전, 회전중심스케일(이동거리유지 x)
             * case5            : 자체회전, 회전중심스케일(이동거리유지 x)
            */
            
            /** @note Case 1 (정상) */
            // model = glm::translate(model, cubePositions[i]);
            // model = glm::rotate(model, (float)timeValue, glm::vec3(0.5f, 1.0f, 0.0f));
            // model = glm::scale(model, glm::vec3(abs(cosVal), abs(cosVal), abs(cosVal)));

            /** @note Case 2 (case1이랑 차이가 없음) */
            // model = glm::translate(model, cubePositions[i]);
            // model = glm::scale(model, glm::vec3(abs(cosVal), abs(cosVal), abs(cosVal)));
            // model = glm::rotate(model, (float)timeValue, glm::vec3(0.5f, 1.0f, 0.0f));


            /** @note Case 3  */
            // model = glm::rotate(model, (float)timeValue, glm::vec3(0.5f, 1.0f, 0.0f));
            // model = glm::translate(model, cubePositions[i]);
            // model = glm::scale(model, glm::vec3(abs(cosVal), abs(cosVal), abs(cosVal)));
            
            /** @note Case 4  */
            // model = glm::rotate(model, (float)timeValue, glm::vec3(0.5f, 1.0f, 0.0f));
            // model = glm::scale(model, glm::vec3(abs(cosVal), abs(cosVal), abs(cosVal)));
            // model = glm::translate(model, cubePositions[i]);


            /** @note Case 5  */
            // model = glm::scale(model, glm::vec3(abs(cosVal), abs(cosVal), abs(cosVal)));
            // model = glm::translate(model, cubePositions[i]);
            // model = glm::rotate(model, (float)timeValue, glm::vec3(0.5f, 1.0f, 0.0f));

            /** @note Case 6 */
            // model = glm::scale(model, glm::vec3(abs(cosVal), abs(cosVal), abs(cosVal)));
            // model = glm::rotate(model, (float)timeValue, glm::vec3(0.5f, 1.0f, 0.0f));
            // model = glm::translate(model, cubePositions[i]);


            // model = glm::rotate(model, (float)timeValue, glm::vec3(0.5f, 1.0f, 0.0f));
            // model = glm::scale(model, glm::vec3(abs(cosVal), abs(cosVal), abs(cosVal)));
            // model = glm::translate(model, cubePositions[i]);
            // model = glm::rotate(model, (float)timeValue * 10, glm::vec3(0.5f, 1.0f, 0.0f));

            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, (float)timeValue, glm::vec3(0.5f, 1.0f, 0.0f));
            // model = glm::scale(model, glm::vec3(abs(cosVal), abs(cosVal), abs(cosVal)));
            
            ourShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(g_mainWindow);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}