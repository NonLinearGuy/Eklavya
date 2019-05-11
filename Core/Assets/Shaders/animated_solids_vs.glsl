#version 430 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec3 bitangent;
layout(location = 4) in vec2 tex;
layout(location = 5) in vec4 weights;
layout(location = 6) in ivec4 boneIds; 


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightProjectionView;


const int MAX_JOINTS = 100;
const int MAX_WEIGHTS = 4;
uniform mat4 gBones[MAX_JOINTS];

out FragParams
{
    mat3 TBN;
	vec2 texCoords;
	vec4 lightSpacePos;
	vec3 viewSpaceFragPos;
	vec3 viewSpaceNormal;
}fragParams;

out vec4 inColor;

void main()
{
   vec4 totalPosition = vec4(0.0f);
   vec3 totalNormal = vec3(0.0f);

   for(int i = 0 ; i < MAX_WEIGHTS ; i++)
   {
   if(boneIds[i] == -1) continue;

   if(boneIds[i] >= MAX_JOINTS)
   {
		totalPosition = vec4(pos,1.0f);
		break;
   }

	vec4 localPosition = gBones[boneIds[i]] * vec4(pos,1.0f);
	totalPosition += localPosition * weights[i];

	vec3 localNormal = mat3(gBones[boneIds[i]]) * norm;
	totalNormal += localNormal * weights[i];
   }
	//totalPosition = vec4(pos,1.0f);
    //totalNormal = norm;
    mat4 viewModel = view * model;
    gl_Position =  projection * viewModel * totalPosition;
	fragParams.lightSpacePos = lightProjectionView * model * totalPosition;
	fragParams.viewSpaceFragPos = vec3(viewModel * totalPosition);
	fragParams.viewSpaceNormal = normalize(mat3(viewModel)*totalNormal);
	fragParams.texCoords = tex;

	vec3 T = normalize(mat3(viewModel) * tangent);
	vec3 B = normalize(mat3(viewModel) * bitangent);
	vec3 N = fragParams.viewSpaceNormal;
	fragParams.TBN = mat3(T,B,N);
}
