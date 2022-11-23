#include <alkagi/alkagi.h>

int main()
{
    /** @todo Enum */
    if (init() == 0) return -1;

    /** @todo root 하드코딩 */
    Shader ourShader("/Users/joseonghyeon/dev/alkagi/src/main.vs", "/Users/joseonghyeon/dev/alkagi/src/main.fs");

    float vertices[] = {
        // positions          // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
    };

    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    uint VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    uint texture1, texture2;
    int width, height, nrChannels;

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    /** @note GL_CLAMP_TO_BORDER 사용시 border color를 설정 */
    // float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  

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
        glClear(GL_COLOR_BUFFER_BIT);
        
        /** @note On some drivers, it is required to assign a texture unit to each sampler uniform */
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        /** @note GL_TEXTUREi == GL_TEXTURE0 + i (Loop할때 유용) */
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // ourShader.use();
        ourShader.setFloat("mixValue", mixValue);


        /**
         * @note
         * It is advised to first do scaling operations,
         * then rotations and lastly translations when combining matrices.
         * otherwise they may (negatively) affect each other. 
         *  
         * For example, if you would first do a translation and then scale,
         * the translation vector would also scale!
        */
        float timeValue = static_cast<float>(abs(sin(glfwGetTime())));

        float cosVal = static_cast<float>(cos(glfwGetTime() * 4));
        float sinVal = static_cast<float>(sin(glfwGetTime() * 4));
        glm::mat4 transform = glm::mat4(1.0f); 
        // transform = glm::translate(transform, glm::vec3(cosVal * 2, sinVal * 2, 0.0f));
        transform = glm::translate(transform, glm::vec3(0.5 * cosVal, 0.5 * sinVal, 0.0f));

        /**
         * @todo 3번째 인자
         * 
         * @note
         * Keep in mind that the axis that we rotate around should be a unit vector, 
         * so be sure to normalize the vector first 
         * if you're not rotating around the X, Y, or Z axis.
        */
        // transform = glm::rotate(transform, (float)glfwGetTime() * 2, glm::vec3(1.0f, 0.0f, 1.0f));
        transform = glm::rotate(transform, (float)glfwGetTime() * 2, glm::vec3(1.0f, 0.0f, 1.0f));
        /** @todo 위에 방식 vs 아래 방식 */
        // transform = glm::rotate(transform, (float)glfwGetTime() * 2, glm::vec3(1.0f, 0.0f, 0.0f));
        // transform = glm::rotate(transform, (float)glfwGetTime() * 2, glm::vec3(0.0f, 0.0f, 1.0f));
        
        /** @todo 왜 x,y에 timevalue를 모두 안넣어주면, scale했을때 안보이는 건지? */
        transform = glm::scale(transform, glm::vec3(timeValue, timeValue, 0.0f));
        // transform = glm::scale(transform, glm::vec3(timeValue, timeValue, timeValue));
        

        ourShader.use();
        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        /**
         * @todo GL_TRUE로 하니까 시계방향됨
         * 
         * @note
         * The third argument asks us if we want to transpose our matrix,
         * that is to swap the columns and rows.
         * 
         * OpenGL developers often use an internal matrix layout called column-major ordering
         * which is the default matrix layout in GLM so there is no need to transpose the matrices; 
         * we can keep it at GL_FALSE.
        */
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        /** @todo 그냥 이렇게 glDrawElements를 한번 더 호출하면 되는 거였음? */
        transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(0.3 * cosVal, 0.3 * sinVal, 0.0f));
        transform = glm::rotate(transform, (float)glfwGetTime() * 3, glm::vec3(1.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3(timeValue, timeValue, 0.0f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        /** @note &transform[0][0] */
        // glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



        transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(0.1 * cosVal, 0.1 * sinVal, 0.0f));
        transform = glm::rotate(transform, (float)glfwGetTime() * 5, glm::vec3(1.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3(timeValue, timeValue, 0.0f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(g_mainWindow);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}