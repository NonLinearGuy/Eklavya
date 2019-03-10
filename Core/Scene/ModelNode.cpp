#include "ModelNode.h"
#include "../Components/BaseRenderComponent.h"
#include <memory>
#include "../Model.h"
#include "Scene.h"
#include "../Renderer/GLRenderer.h"
#include "../Components/AnimationComponent.h"
#include "../Engine.h"
#include "../Helpers.h"

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
	std::string modelName = (static_cast<MeshRenderComponent*>(m_WeakRenderComponent))->GetModelName();
	m_Model = new Model("Assets/Models/" + modelName);
	
	return true;
}

void ModelNode::Destroy()
{
	
}

void ModelNode::Render(Scene * scene)
{
	auto shader = scene->GetRenderer()->GetActiveProgram();
	std::shared_ptr<GameActor> gameActor = scene->GetEngineRef()->GetActor(m_ActorID);
	std::shared_ptr<AnimationComponent> transform = MakeSharedPtr(gameActor->GetComponent<AnimationComponent>(AnimationComponent::s_ID));
	if (transform)
	{
		auto boneMap = Model::m_BoneIdMap;
		auto transforms = transform->GetPoseTransforms();
		for (int i = 0; i < transforms.size(); ++i)
			shader->SetMat4("gBones[" + std::to_string(i) + "]", transforms[i]);
	}
	m_Model->Draw(shader);
}
