#include "Material.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

Material::Material()
	:
	m_Ambient(0.3f),m_Diffuse(1.0f),m_Specular(.5f),m_SpecPow(16)
{
}

Material::~Material()
{
}

void Material::SetAlbedoMap(std::shared_ptr<Texture2D> pAlbedo)
{
	m_Albedo = pAlbedo;
}

void Material::SetPropsInShader(std::shared_ptr<ShaderProgram> pShader)
{
	if (!pShader->IsCurrentlyActive())
	{
		pShader->Use();
		pShader->SetStatus(true);
	}

	pShader->SetInt("material.bApplyColor",m_UseColor);
	pShader->SetVec3("material.ambient",m_Ambient);
	pShader->SetVec3("material.diffuse", m_Diffuse);
	pShader->SetVec3("material.specular", m_Specular);
	pShader->SetFloat("material.specPow",m_SpecPow);
	pShader->SetFloat("material.opacity", m_Opacity);

	if (m_Albedo)
	{
		pShader->SetInt("material.albedoMap",0);
		m_Albedo->BindToUnit(GL_TEXTURE0);
	}
}
