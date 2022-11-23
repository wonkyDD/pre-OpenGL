// @note
// Inversing matrices is a costly operation for shaders, 
// so wherever possible try to avoid doing inverse operations 
// since they have to be done on each vertex of your scene. 
// For learning purposes this is fine, but for an efficient application 
// you'll likely want to calculate the normal matrix on the CPU 
// and send it to the shaders via a uniform before drawing (just like the model matrix).


// @note    vs변화
// 
// 1. lightPos를 fs에서 vs로 유니폼 위치변경
// 2. FragPos 변경: view * model (월드가 아닌 뷰공간에서 논의)
// 3. LightPos 추가 (lightPos랑 다름)


#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;
out vec3 LightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;


void main()
{
    Normal = mat3(transpose(inverse(view * model))) * aNormal;
    // LightPos = mat3(view) * lightPos;
    LightPos = vec3(view * vec4(lightPos, 1.0));
    FragPos = vec3(view * model * vec4(aPos, 1));
    gl_Position = projection * vec4(FragPos, 1);
}