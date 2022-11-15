#version 330 core
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixValue;

in vec2 TexCoord;

void main()
{
    // @note vertex color와 texture color 믹스
    // FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixValue) * vec4(ourColor, 1.0);
    
    // @note 
    // TexCoord.x  ->  1 - TexCoord.x 또는
    // TexCoord.y  ->  1 - TexCoord.y 로 변경해서 뒤집기
    // FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1-TexCoord.x, TexCoord.y)), mixValue);
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixValue);
}