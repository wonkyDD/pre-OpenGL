#version 330 core
out vec4 FragColor;


struct Material
{
    float shininess;
    sampler2D diffuse;
    sampler2D specular;
};

struct Light
{
    vec3 position;
    vec3 direction;
    float cutOff;
    // float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;


void main()
{
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir, -light.direction);

    if (theta > light.cutOff)
    {
        // ambient
        vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

        // diffuse
        vec3 norm = normalize(Normal);
        // vec3 lightDir = normalize(-light.direction);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;

        // specular
        vec3 reflectDir = reflect(-lightDir, norm);
        vec3 viewDir = normalize(viewPos - FragPos);
        float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
        vec3 specular =  light.specular * spec * texture(material.specular, TexCoord).rgb;

        // attenuation
        float d = length(light.position - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * d + light.quadratic * d * d);

        // remove attenuation from ambient, 
        // as otherwise at large distances the light would be darker inside 
        // than outside the spotlight due the ambient term in the else branche

        // ambient  *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;

        vec3 result = ambient + diffuse + specular;
        FragColor = vec4(result, 1.0);
    }
    else
    {
        FragColor = vec4(light.ambient * texture(material.diffuse, TexCoord).rgb, 1.0);
    }
}