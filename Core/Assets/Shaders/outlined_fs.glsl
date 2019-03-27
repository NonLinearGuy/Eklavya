#version 430 core

out vec4 finalColor;

uniform vec4 color;

in vec3 fragColor;

void main()
{
	finalColor = vec4(fragColor,1.0f); 
}