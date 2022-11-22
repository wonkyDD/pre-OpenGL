#version 330 core
out vec4 FragColor;


struct Material
{
    float shininess;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
};

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform float time;
uniform float r;
uniform float g;
uniform float b;


void main()
{
    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;

    // specular
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(viewPos - FragPos);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    vec3 specular =  light.specular * spec * texture(material.specular, TexCoord).rgb;

    // emission
    vec3 emissionIntensity = vec3(abs(sin(time * 0.2)));
    vec3 emission = texture(material.emission, vec2(TexCoord.x, TexCoord.y + time*2.0)).rgb;


    // vec3 emission_result = vec3(1.0);
    // if (sin(time) >=0 && sin(time) < 0.2) 
    // {
    //     emission_result = vec3(emission.r, emission.g, emission.g);
    // }
    // else if(sin(time) >=0.2 && sin(time) < 0.4)
    // {
    //     emission_result = vec3(emission.r, emission.b, emission.b);
    // }
    // else if(sin(time) >=0.4 && sin(time) <= 0.6)
    // {
    //     emission_result = vec3(emission.r, emission.r, emission.g);
    // }
    // else if(sin(time) >=0.6 && sin(time) <= 0.8)
    // {
    //     emission_result = vec3(emission.g, emission.g, emission.b);
    // }
    // else
    // {
    //     emission_result = vec3(emission.r, emission.g, emission.b);
    // }
    // vec3 result = ambient + diffuse + specular + emission_result * emissionIntensity;


    // vec3 result = ambient + diffuse + specular + vec3(emission.r * r, emission.g * g, emission.b * b) * emissionIntensity;
    vec3 result = ambient + diffuse + specular + vec3(emission.g * r, emission.g * g, emission.b * b) * emissionIntensity;
    // vec3 result = ambient + diffuse + specular + vec3(emission.r * r, emission.g * g, emission.g * b) * emissionIntensity;
    FragColor = vec4(result, 1.0);
}