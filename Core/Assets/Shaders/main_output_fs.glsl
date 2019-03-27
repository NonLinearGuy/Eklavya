#version 430 core

in vec2 texCoords;
uniform sampler2D mainPassOutput;

out vec4 finalColor;

void main()
{
vec2 tex = texCoords;
    tex.y = 1.0f - texCoords.y;
	finalColor = texture(mainPassOutput,tex);
}