#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
// uniform float time;

void main()
{
    Normal = mat3(transpose(inverse(model))) * aNormal;
    // FragPos = vec3(model * vec4(aPos + abs(sin(time * 0.5 + TexCoord.y)) * abs(cos(time * 0.5 + TexCoord.x)) * Normal, 1.0));
    FragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(FragPos, 1.0);
    TexCoord = aTexCoord;
}