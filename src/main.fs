#version 330 core
out vec4 FragColor;

// iTime을 구현
uniform float time;

in vec3 ourColor;
in vec2 ourPos;

void main()
{
    // vec3 col = 0.5 + 0.5 * cos(time + ourPos.xyx);
    vec3 col = 0.5 + 0.5 * cos(time + ourPos.xyx + vec3(0, 2, 4));
    FragColor = vec4(col, 1.0);
}