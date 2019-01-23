#pragma once

#include<glm/glm.hpp>
#include <memory>

class Texture2D;
class ShaderProgram;

class Material
{
public:
	Material();
	~Material();

	inline void SetUseColor(bool UseColor) { m_UseColor = UseColor; }
	inline void SetAmbient(const glm::vec3& ambient) { m_Ambient = ambient; }
	inline void SetDiffuse(const glm::vec3& diffuse) { m_Diffuse = diffuse; }
	inline void SetSpecular(const glm::vec3& specular) { m_Specular = specular; }
	inline void SetSpecPow(float pow) { m_SpecPow = pow; }
	inline void SetOpacity(float opacity) { m_Opacity = opacity; }

	void SetNormalMap(std::shared_ptr<Texture2D> pNormalMap);
	void SetAlbedoMap(std::shared_ptr<Texture2D> pAlbedoMap);
	void SetPropsInShader(std::shared_ptr<ShaderProgram> pShader);
	
private:
	std::shared_ptr<Texture2D> m_Albedo;
	std::shared_ptr<Texture2D> m_Normal;
	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular; 
	float m_Opacity;
	float m_SpecPow;
	bool m_UseColor;
};
