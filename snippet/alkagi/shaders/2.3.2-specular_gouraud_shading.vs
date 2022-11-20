// @note
// Inversing matrices is a costly operation for shaders, 
// so wherever possible try to avoid doing inverse operations 
// since they have to be done on each vertex of your scene. 
// For learning purposes this is fine, but for an efficient application 
// you'll likely want to calculate the normal matrix on the CPU 
// and send it to the shaders via a uniform before drawing (just like the model matrix).

// #version 330 core
// layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aNormal;

// out vec3 Normal;
// out vec3 FragPos;
// out vec3 LightPos;

// uniform mat4 model;
// uniform mat4 view;
// uniform mat4 projection;
// uniform vec3 lightPos;


// void main()
// {
//     Normal = mat3(transpose(inverse(view * model))) * aNormal;
//     LightPos = vec3(view * vec4(lightPos, 1.0));
//     FragPos = vec3(view * model * vec4(aPos, 1));
//     gl_Position = projection * vec4(FragPos, 1);
// }

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 LightColor;

uniform float strength;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
    vec3 Position = vec3(model * vec4(aPos, 1.0));
    vec3 Normal = mat3(transpose(inverse(model))) * aNormal;

    // ambient
    float ambientStrength = strength;
    vec3 ambient = strength * lightColor;

    // diffuse
    vec3 lightDir = normalize(lightPos - Position);
    // vec3 Normal = normalize(aNormal);
    vec3 norm = normalize(Normal);
    vec3 diffuse = max(dot(lightDir, norm), 0.0) * lightColor;

    // specular
    float specularStrength = 1.0;
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(viewPos - Position);
    vec3 specular = pow(max(dot(reflectDir, viewDir), 0.0), 32) * specularStrength * lightColor;

    LightColor = (ambient + diffuse + specular);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}