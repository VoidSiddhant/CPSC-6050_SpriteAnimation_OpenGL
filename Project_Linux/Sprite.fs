#version 450 core

in vec2 TexCoords;
out vec4 FragColor;
uniform sampler2D mainTex;

uniform float uvX;
uniform float uvY;
uniform float nxFrame;
uniform float nyFrame;

void main()
{
    float x = 1.0f / nxFrame;
    float y = 1.0f / nyFrame;
    vec4 result = texture(mainTex,vec2(TexCoords.x * x, TexCoords.y * y) + vec2(x * uvX , y*uvY));
    //if(result.r <= 0.2)
    //    result.a = 0;
    FragColor = result;
}