#version 330 core

layout(location = 0) out vec4 fragColor;
in vec3 fragPos;
in vec4 clipSpace;
in vec2 texCoords;

uniform vec3 cameraPosition;
uniform sampler2D refractionTexture;
uniform sampler2D reflectionTexture;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;
uniform float moveOffset;

const float WAVE_STRENGTH =.02;
vec3 lightColor = vec3(1.0f,1.0f,1.0f);

void main()
{
vec3 viewDirection = normalize(cameraPosition - fragPos);
	vec2 ndc = (clipSpace.xy/clipSpace.w)/2.0f + 0.5f;
	
	vec2 refractTexCoord = vec2(ndc.x,ndc.y);
	vec2 reflectTexCoord = vec2(ndc.x,ndc.y);

	vec2 distortedTexCoords = (texture(dudvMap,vec2(texCoords.x + moveOffset,texCoords.y))).rg * .04;
	distortedTexCoords = texCoords + vec2(distortedTexCoords.x,distortedTexCoords.y + moveOffset);
	vec2 totalDistortion  = (texture(dudvMap,distortedTexCoords).rg * 2.0f - 1.0f) * WAVE_STRENGTH;
	refractTexCoord += totalDistortion;
	reflectTexCoord += totalDistortion;

	vec4 refractColor = texture(refractionTexture,refractTexCoord);
	vec4 reflectColor = texture(reflectionTexture,reflectTexCoord);

	float refractiveIndex = pow(dot(viewDirection,vec3(0.0f,1.0f,0.0f)),1.0f);
	
	vec3 normalColor = texture(normalMap,glm::vec2(0.0f,1.0f) - distortedTexCoords).rgb;
	vec3 normal = vec3(normalColor.r * 2.0f - 1.0f, normalColor.g * 2.0f - 1.0f, normalColor.b * 2.0f - 1.0f);
	normal = normalize(normal);
	vec3 reflectDir = reflect(normalize(vec3(1.0f)),normal);
	vec3 tempViewDirection = viewDirection;
	tempViewDirection.z = tempViewDirection.z * -1;
	float spec = pow(max(dot(tempViewDirection, reflectDir), 0.0), 264);
	vec3 specular = 1.0 * spec * lightColor;  

	//fragColor = mix(refractColor,reflectColor,refractiveIndex) ;
	fragColor = mix(refractColor,vec4(0.0f,0.4,1.0f,2.0f),.5f) + vec4(specular,0.0);	
}