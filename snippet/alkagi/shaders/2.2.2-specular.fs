#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform float strength;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;


// @note
// We chose to do the lighting calculations in world space, 
// but most people tend to prefer doing lighting in view space. 
// An advantage of view space is that the viewer's position is always at (0,0,0) 
// so you already got the position of the viewer for free. 
// However, I find calculating lighting in world space more intuitive for learning purposes. 
// If you still want to calculate lighting in view space,
// you want to transform all the relevant vectors with the view matrix as well 
// (don't forget to change the normal matrix too).


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

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    // @todo 32라는 숫자 (광원의 응집도 <<< shininess value of the highlight)  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
            
    // vec3 result = (ambient + diffuse) * objectColor;
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}