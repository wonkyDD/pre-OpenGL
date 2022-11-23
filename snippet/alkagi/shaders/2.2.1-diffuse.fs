#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform float strength;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
    // ambient
    // float ambientStrength = 0.1;
    float ambientStrength = strength;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
            
    // vec3 result = (diffuse) * objectColor;
    // vec3 result = (ambient) * objectColor;
    // vec3 result = (ambientStrength + diff) * lightColor * objectColor;
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}