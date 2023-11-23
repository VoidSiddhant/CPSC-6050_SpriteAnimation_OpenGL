#version 450 core

in vec2 TexCoords;
out vec4 FragColor;
uniform sampler2D mainTex;

uniform float tileX;
uniform float tileY;

void main()
{
    vec4 result = texture(mainTex, vec2(TexCoords.x * tileX, TexCoords.y * tileY));
    //if(result.r <= 0.2)
    //    result.a = 0;
    FragColor = result;
}