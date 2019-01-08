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
};


uniform Material material;
uniform float opacity;

void main()
{
	if(material.bApplyColor)
		finalColor = vec4(material.diffuse,opacity);
	else
		finalColor = texture(material.albedoMap,fragParams.texCoords);
	
}