#pragma once

#include "BaseNode.h"

class RootNode : public BaseNode
{
public:
	RootNode();
	~RootNode();

	void AddChild(std::shared_ptr<BaseNode> pChild)override;
	bool RemoveChild(std::shared_ptr<BaseNode> pChild)override;
	void RenderChildren(class Scene* scene)override;
private:
};