// @note    fs변화
//
// 1. 기존유니폼 lightPos, viewPos 주석처리
// - LightPos로 vs에서 받아오고 (view 행렬을 곱해서)
// - viewPos는 결국 view공간이기에 viewPos가 vec3(0.0, 0.0, 0.0)으로 픽스되기때문
//
// 2. 그에 맞게 수정
// - vec3 lightDir = normalize(lightPos - FragPos)을
// vec3 lightDir = normalize(LightPos - FragPos)으로 수정
//
// - vec3 viewDir = normalize(viewPos-FragPos)을
// vec3 viewDir = normalize(-FragPos)으로 수정


#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

uniform float strength;
// uniform vec3 lightPos;
// uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;


void main()
{
    // ambient
    // float ambientStrength = 0.1;
    float ambientStrength = strength;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
            
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}