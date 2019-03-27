#version 430 core

out vec4 finalColor;

in FragParams
{
	vec2 texCoords;
	vec4 lightSpacePos;
	vec3 viewSpaceFragPos;
	vec3 viewSpaceNormal;
}fragParams;

struct Material
{
	sampler2D albedoMap;
	sampler2D normalMap;
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

float GetShadowValue(float cosTheta)
{
	vec4 sampleCoords = fragParams.lightSpacePos * 0.5 + 0.5; 
 	float bias = max(0.009 * (1.0 - cosTheta), 0.005);  
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowPassOutput, 0);

	if(fragParams.lightSpacePos.z > 1.0f)
		return 0.0f;

	for(int x = -4; x <= 4; ++x)
	{
		for(int y = -4; y <= 4; ++y)
		{
			float pcfDepth = texture(shadowPassOutput, sampleCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += sampleCoords.z < pcfDepth + bias ? 0.0 : 1.0;        
		}		 
	}


	return (shadow / 81.0f );
}

void main()
{
    vec3 normal = fragParams.viewSpaceNormal;
	if(material.bApplyNormalMap)
	{
		//normal = texture(material.normalMap,fragParams.texCoords).rgb;
		//normal = normalize(normal*2.0 - 1.0); // [-1,1] ---> [0,1]
		//normal = normalize(fragParams.TBN * normal);
	}

	vec3 ambient,diffuse,specular;
	vec3 lightDirection = normalize(light.position - fragParams.viewSpaceFragPos);
	float diff = dot(lightDirection,fragParams.viewSpaceNormal);
		
	if(!material.bApplyColor)
	{
		vec3 color = texture(material.albedoMap,fragParams.texCoords).rgb;
		ambient = light.ambient * color * .6;
		diffuse = color * diff * light.diffuse;
	}
	else
	{
		ambient = material.ambient * .2;
		diffuse = material.diffuse * .6;
	}

	float shadow = diff < -.5f ? 0.0f : GetShadowValue(diff);
	finalColor = vec4((1.0f - shadow) * (diffuse + ambient),material.opacity);
	
}