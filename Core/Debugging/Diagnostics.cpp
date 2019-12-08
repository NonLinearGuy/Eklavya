#include "Diagnostics.h"
#include "../UI/TextRenderer.hpp"
#include <GLFW/glfw3.h>
#include <sstream>

std::map<EMapKeys, std::string> DiagManager::sSceneDiagsMap;
std::map<EMapKeys, std::string> DiagManager::sPhysicsDiagsMap;
std::map<EMapKeys, std::string> DiagManager::sGeneralDiagsMap;

DiagManager::DiagManager(std::shared_ptr<TextRenderer> Text)
	:
	m_TextRenderer(Text),
	m_CurrentTab(ETabs::GENERAL)
{
	m_KeyStrings.reserve(EMapKeys::KEY_MAX);
	
	for (int keyIndex = 0; keyIndex < EMapKeys::KEY_MAX; ++keyIndex)
		m_KeyStrings.push_back("");
	
	//General
	m_KeyStrings[EMapKeys::KEY_FPS] = "Fps";
	m_KeyStrings[EMapKeys::KEY_DELTA] = "Delta";

	//Physics 
	m_KeyStrings[EMapKeys::KEY_COLLIDERS] = "Colliders";
	m_KeyStrings[EMapKeys::KEY_INTERPEN] = "Penetration";
	m_KeyStrings[EMapKeys::KEY_NORMAL] = "Normal";
	m_KeyStrings[EMapKeys::KEY_CONTACT_POINT] = "Contact Point";
	//Scene Graph
	m_KeyStrings[EMapKeys::KEY_CULLED_OBJECT_COUNT] = "Node Culled";
	m_KeyStrings[EMapKeys::KEY_SOLID_GROUP_OBJECTS_COUNT] = "Total Solids";

}

DiagManager::~DiagManager()
{
	m_TextRenderer.reset();
}

void DiagManager::OnKeyAction(int key, int action)
{
	if (action == GLFW_PRESS)
	{
		if (GLFW_KEY_PAGE_DOWN == key) --m_CurrentTab;
		if (GLFW_KEY_PAGE_UP == key) ++m_CurrentTab;
		m_CurrentTab = (m_CurrentTab < 0) ? ETabs::TAB_MAX - 1 : (m_CurrentTab % ETabs::TAB_MAX);
	}
}

void DiagManager::PostCurrent()
{
	m_Stream.str("");
	switch (m_CurrentTab)
	{
	case ETabs::GENERAL:
		m_Stream << "[     GENERAL    ]\n\n";
		PostGeneralDiags();
		break;
	case ETabs::PHYSICS:
		m_Stream << "[     PHYSICS    ]\n\n";
		PostPhysicsDiags();
		break;
	case ETabs::SCENE_GRAPH:
		m_Stream << "[      SCENE GRAPH     ] \n\n";
		PostSceneGraphDiags();
		break;
	};
	m_Stream << "=======================";
	m_TextRenderer->Text(glm::vec2(200.0f), 2.0f, glm::vec3(0.0f, 0.0f, 0.0f), m_Stream.str().c_str());
	
}

void DiagManager::PostGeneralDiags()
{
	for (auto pair : sGeneralDiagsMap)
		m_Stream << m_KeyStrings[pair.first] << " = " << pair.second << "\n";
}

void DiagManager::PostSceneGraphDiags()
{
	for (auto pair : sSceneDiagsMap)
		m_Stream << m_KeyStrings[pair.first] << " = " << pair.second << "\n";
}


void DiagManager::PostPhysicsDiags()
{
	for (auto pair : sPhysicsDiagsMap)
		m_Stream << m_KeyStrings[pair.first] << " = " << pair.second << "\n";
}

