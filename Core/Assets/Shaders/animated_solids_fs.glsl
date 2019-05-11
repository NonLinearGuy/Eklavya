#version 430 core

out vec4 finalColor;
in vec4 inColor;

in FragParams
{
	mat3 TBN;
	vec2 texCoords;
	vec4 lightSpacePos;
	vec3 viewSpaceFragPos;
	vec3 viewSpaceNormal;
}fragParams;

struct Material
{
	sampler2D texture_diffuse;
	sampler2D texture_specular;
	sampler2D texture_height;
	sampler2D texture_normal;
	bool bApplyColor;
	bool bApplyNormalMap;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float specPow;
	float opacity;
};

struct Light
{
	vec3 position;
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
};

uniform Light light;
uniform Material material;
uniform sampler2D shadowPassOutput;

void main()
{


	vec3 normal = fragParams.viewSpaceNormal;
		if(material.bApplyNormalMap)
		{
			normal = texture(material.texture_normal,fragParams.texCoords).rgb;
			normal = normalize(normal*2.0 - 1.0); // [-1,1] ---> [0,1]
			normal = normalize(fragParams.TBN * normal);
		}
	
 
	vec3 ambient,diffuse,specular;
	vec3 lightDirection = normalize(light.position - fragParams.viewSpaceFragPos);
	float diff = dot(lightDirection,normal);
	
	vec3 viewDir    = normalize(-fragParams.viewSpaceFragPos);
	vec3 halfwayDir = normalize(lightDirection + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 1024);
	
	specular =  texture(material.texture_specular,fragParams.texCoords).rgb * spec * light.specular;
		
	vec3 color = texture(material.texture_diffuse,fragParams.texCoords).rgb;
	ambient = light.ambient * color * .8;
	diffuse = color * diff * light.diffuse;
	
	finalColor = vec4(ambient + diffuse + specular,1.0f);
}