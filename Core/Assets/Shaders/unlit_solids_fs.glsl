#version 430 core

out vec4 finalColor;

in FragParams
{
	vec2 texCoords;
}fragParams;

struct Material
{
	sampler2D albedoMap;
	bool bApplyColor;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float specPow;
	float opacity;
};


uniform Material material;

void main()
{
	if(material.bApplyColor)
		finalColor = vec4(material.diffuse,material.opacity);
	else
		finalColor = texture(material.albedoMap,fragParams.texCoords);	
}