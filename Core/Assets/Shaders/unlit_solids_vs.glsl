#version 430 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 tex;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out FragParams
{
	vec2 texCoords;
}fragParams;

void main()
{	
    gl_Position =  projection * view * model * vec4(pos,1.0f);    
	fragParams.texCoords = tex;
}