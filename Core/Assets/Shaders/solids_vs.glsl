#version 430 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 tex;


uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform mat4 lightProjectionView;

out FragParams
{
	vec2 texCoords;
	vec4 lightSpacePos;
	vec3 viewSpaceFragPos;
	vec3 viewSpaceNormal;
}fragParams;


void main()
{	
	vec3 tangent = vec3(1.0f,0.0f,0.0f);
	vec3 bitangent = vec3(0.0f,1.0f,0.0f);
	vec3 normal = vec3(0.0f,0.0f,1.0f);

	mat4 viewModel = view * model;
    gl_Position =  projection * viewModel * vec4(pos,1.0f);    
	fragParams.lightSpacePos = lightProjectionView * model * vec4(pos,1.0f);
	fragParams.viewSpaceFragPos = vec3(viewModel * vec4(pos,1.0f));
	fragParams.viewSpaceNormal = normalize(mat3(viewModel) * norm);
	fragParams.texCoords = tex;
}