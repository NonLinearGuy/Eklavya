#pragma once

#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "../UserInputListener.h"
#include "../Globals.h"

class TextRenderer;

enum ETabs
{
	TAB_MIN = 0,
	GENERAL = TAB_MIN, //FPS & delta
	PHYSICS, //will post octree execution time, total physics objects, total collision pairs etc
	SCENE_GRAPH,//will tell total scene objects rendered, total objects culled etc etc
	TAB_MAX
};

enum EMapKeys
{
	//GENERAL
	KEY_FPS = 0,
	KEY_DELTA,

	//PHYSICS,
	KEY_COLLIDERS,
	KEY_INTERPEN,
	KEY_NORMAL,
	KEY_CONTACT_POINT,
	//SCENE GRAPH
	KEY_CULLED_OBJECT_COUNT,
	KEY_SOLID_GROUP_OBJECTS_COUNT,
	KEY_BOUND_VOLUME_GROUP_COUNT,
	KEY_MAX
};

class DiagManager : public UserInputListener
{
public:
	DiagManager(std::shared_ptr<TextRenderer> Text);
	~DiagManager();
	void OnKeyAction(int key, int action)override;
	void PostCurrent();
public:
	//inline static void SetKeyData(EMapKeys key);
	static std::map<EMapKeys, std::string> sSceneDiagsMap;
	static std::map<EMapKeys, std::string> sPhysicsDiagsMap;
	static std::map<EMapKeys, std::string> sGeneralDiagsMap;
private:

	void PostGeneralDiags();
	void PostSceneGraphDiags();
	void PostPhysicsDiags();
	std::shared_ptr<TextRenderer> m_TextRenderer;
	short unsigned m_CurrentTab;
	std::stringstream m_Stream;
	std::vector<std::string> m_KeyStrings;
};


						