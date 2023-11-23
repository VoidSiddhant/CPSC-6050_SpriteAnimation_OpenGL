#version 450 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;

void main()
{
	gl_Position = projection * model * vec4(aPos.x,aPos.y,0.0f,1.0f);
	TexCoords = vec2(aTexCoords.x,aTexCoords.y);
}