// #version 330 core
// out vec4 FragColor;

// in vec3 Normal;
// in vec3 FragPos;
// in vec3 LightPos;

// uniform float strength;
// uniform vec3 lightColor;
// uniform vec3 objectColor;


// void main()
// {
//     // ambient
//     // float ambientStrength = 0.1;
//     float ambientStrength = strength;
//     vec3 ambient = ambientStrength * lightColor;
  	
//     // diffuse 
//     vec3 norm = normalize(Normal);
//     vec3 lightDir = normalize(LightPos - FragPos);
//     float diff = max(dot(norm, lightDir), 0.0);
//     vec3 diffuse = diff * lightColor;

//     // specular
//     float specularStrength = 0.5;
//     vec3 viewDir = normalize(-FragPos);
//     vec3 reflectDir = reflect(-lightDir, norm);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
//     vec3 specular = specularStrength * spec * lightColor;  
            
//     vec3 result = (ambient + diffuse + specular) * objectColor;
//     FragColor = vec4(result, 1.0);
// }

#version 330 core
out vec4 FragColor;
in vec3 LightColor;

uniform vec3 objectColor;

void main()
{
    FragColor = vec4(LightColor * objectColor, 1.0);
}