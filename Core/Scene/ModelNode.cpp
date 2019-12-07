#include "ModelNode.h"
#include "../Components/BaseRenderComponent.h"
#include <memory>
#include "../AssetManager/Model.h"
#include "Scene.h"
#include "../Renderer/GLRenderer.h"
#include "../Components/AnimationComponent.h"
#include "../Engine.h"
#include "../Helpers.h"
#include "BoundingVolume.h"
#include "../AssetManager/AssetManager.h"

ModelNode::ModelNode(ActorID pActorID, BaseRenderComponent * renderComponent, ERenderGroup renderPass)
	:
	BaseNode(pActorID,renderComponent,renderPass)
{
}

ModelNode::~ModelNode()
{
}

bool ModelNode::Init()
{
	std::string name =  (static_cast<MeshRenderComponent*>(m_WeakRenderComponent))->GetModelName();
	m_Model = AssetManager::GetInstance().GetAsset<Model>(name);
	m_BoundVolume = std::make_shared<BoxBound>(glm::vec3(550.0f,900.0f,100.0f));
	return true;
}

void ModelNode::Destroy()
{
	
}

void ModelNode::Render(Scene * scene)
{

	auto shader = scene->GetRenderer()->GetActiveProgram();
	shader->SetInt("material.bApplyNormalMap",mApplyNormaMap);
	std::shared_ptr<GameActor> gameActor = scene->GetEngineRef()->GetActor(m_ActorID);
	std::shared_ptr<AnimationComponent> transform = MakeSharedPtr(gameActor->GetComponent<AnimationComponent>(AnimationComponent::s_ID));
	if (transform)
	{
		auto transforms = transform->GetPoseTransforms();
		for (int i = 0; i < transforms.size(); ++i)
			shader->SetMat4("gBones[" + std::to_string(i) + "]", transforms[i]);
	}
	m_Model->Render(shader);
}

void ModelNode::OnKeyAction(int key, int action)
{
	if (action == GLFW_PRESS && key == GLFW_KEY_N)
		mApplyNormaMap = !mApplyNormaMap;
}
