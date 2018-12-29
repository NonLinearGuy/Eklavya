#pragma once

#include "BaseScene.h"
#include "SceneNode.h"
#include<vector>

using ActorID = unsigned int;

class SceneNode
{
public:
	SceneNode(ActorID id);
	~SceneNode();

	virtual bool Init() = 0;
	virtual bool Destroy() = 0;
	virtual void Tick(float deltaTime) = 0;
	virtual void PreRender() = 0;
	virtual void Render() = 0;
	virtual void PostRender() = 0;

protected:
	ActorID m_ActorID;
	std::list<std::shared_ptr<SceneNode>> m_Children;
	std::shared_ptr<HipHop::BaseScene> m_Scene;
};


//Camera Node : will render frustum

//Model Node : will take the path to model, load it and render it

//AABB render Node : Renderes a box

