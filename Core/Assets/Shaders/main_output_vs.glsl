#version 430 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 tex;

out vec2 texCoords;
uniform mat4 model;
uniform mat4 projection;

void main()
{
	gl_Position = projection * model * vec4(pos,0.0f,1.0f);
	texCoords = tex;
}