#include "FrustumNode.h"
#include "../Renderer/VertexArrayObject.h"
#include "../Renderer/ShaderProgram.h"
#include "Scene.h"
#include "../Renderer/GLRenderer.h"
#include "DebugCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>


FrustumNode::FrustumNode(float fov, float ratio, float fNear, float fFar)
	:
	BaseNode(1000,nullptr,ERenderGroup::OUTLINED)
{
	float halfOfFov = fov / static_cast<float>(2);

	m_NearHeight = 2 * tan(glm::radians(halfOfFov)) * fNear;
	m_NearWidth = m_NearHeight * ratio;

	m_FarHeight = 2 * tan(glm::radians(halfOfFov)) * fFar;
	m_FarWidth = m_FarHeight * ratio;

	glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);

	glm::vec3 nc = m_Points[NEAR_CENTER] = forward * fNear; //assuming camera is at (0,0,0)
	glm::vec3 fc = m_Points[FAR_CENTER] = forward * fFar; //assuming camera is at (0,0,0)

	m_Points[NEAR_TOP_LEFT] = nc + (up * (m_NearHeight / 2.0f)) - (right * (m_NearWidth / 2.0f));
	m_Points[NEAR_TOP_RIGHT] = m_Points[NEAR_TOP_LEFT] + (right * m_NearWidth);
	m_Points[NEAR_BOTTOM_LEFT] = m_Points[NEAR_TOP_LEFT] - (up * m_NearHeight);
	m_Points[NEAR_BOTTOM_RIGHT] = m_Points[NEAR_BOTTOM_LEFT] + (right * m_NearWidth);

	m_Points[FAR_TOP_LEFT] = fc + (up * (m_FarHeight / 2.0f)) - (right * (m_FarWidth / 2.0f));
	m_Points[FAR_TOP_RIGHT] = m_Points[FAR_TOP_LEFT] + (right * m_FarWidth);
	m_Points[FAR_BOTTOM_LEFT] = m_Points[FAR_TOP_LEFT] - (up * m_FarHeight);
	m_Points[FAR_BOTTOM_RIGHT] = m_Points[FAR_BOTTOM_LEFT] + (right * m_FarWidth);

	glm::mat4 projection = glm::perspective(fov,ratio,fNear,fFar);

	CreatePlane(projection);
	
	glm::vec3 vertices[8];
	float yOffset = 20.0f;


	m_Shader = new ShaderProgram();
	m_Shader->AddAndCompile("frustum_vs.glsl", EShaderType::VERTEX);
	m_Shader->AddAndCompile("frustum_fs.glsl", EShaderType::FRAGMENT);
	m_Shader->Build();

	vertices[0] = m_Points[NEAR_BOTTOM_LEFT] + glm::vec3(0.0f, yOffset, 0.0f);
	vertices[1] = m_Points[NEAR_BOTTOM_RIGHT] + glm::vec3(0.0f, yOffset, 0.0f);;
	vertices[2] = m_Points[NEAR_TOP_LEFT] + glm::vec3(0.0f, yOffset, 0.0f);;
	vertices[3] = m_Points[NEAR_TOP_RIGHT] + glm::vec3(0.0f, yOffset, 0.0f);;

	vertices[4] = m_Points[FAR_BOTTOM_LEFT] + glm::vec3(0.0f, yOffset, 0.0f);;
	vertices[5] = m_Points[FAR_BOTTOM_RIGHT] + glm::vec3(0.0f, yOffset, 0.0f);;
	vertices[6] = m_Points[FAR_TOP_LEFT] + glm::vec3(0.0f, yOffset, 0.0f);;
	vertices[7] = m_Points[FAR_TOP_RIGHT] + glm::vec3(0.0f, yOffset, 0.0f);;

	/* init_resources */
	int indices[] = {
		// front
		0, 1,
		0, 2,
		1, 3,
		2, 3,
		// back

		4, 5,
		4, 6,
		5, 7,
		6, 7,

		//middle
		0,4,
		1,5,
		2,6,
		3,7

	};

	m_VAO = new VertexArrayObject();
	m_VAO->Create(&vertices[0], sizeof(glm::vec3) * 8, &indices[0],sizeof(indices));
	m_VAO->SetPosPtr(3, 0, 0);
}

FrustumNode::~FrustumNode()
{
	m_VAO->Destroy();
}

void FrustumNode::CreatePlane(glm::mat4 projection)
{
	glm::vec4 yOffset = glm::vec4(0.0f,0.0f,0.0f,0.0f);
	float a,b,c;
	float d;
	
	// Extracting the planes.

	glm::mat4 matrix = projection;

	glm::vec4 rowX = glm::row(matrix, 0);
	glm::vec4 rowY = glm::row(matrix, 1);
	glm::vec4 rowZ = glm::row(matrix, 2);
	glm::vec4 rowW = glm::row(matrix, 3);

	planes[0] = -glm::normalize(rowW + rowX) + yOffset;
	planes[1] = -glm::normalize(rowW - rowX) + yOffset;
	planes[2] = -glm::normalize(rowW + rowY) + yOffset;
	planes[3] = -glm::normalize(rowW - rowY) + yOffset;
	planes[4] = -glm::normalize(rowW + rowZ) + yOffset;
	planes[5] = -glm::normalize(rowW - rowZ) + yOffset;

}


void FrustumNode::PreRender(Scene * scene)
{
	m_Shader->Use();
	glm::mat4 projection = scene->GetProjection();
	glm::mat4 view = scene->GetCamera()->GetView();
	m_Shader->SetMat4("view",view);
	m_Shader->SetMat4("projection", projection);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_WIDTH);
	glLineWidth(2);

}

void FrustumNode::Render(Scene * scene)
{
	m_VAO->Bind();
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
	m_VAO->Unbind();
}

void FrustumNode::PostRender(Scene * scene)
{
	glDisable(GL_BLEND);
	glDisable(GL_LINE_WIDTH);
}

bool FrustumNode::SphereInsideFrustum(glm::vec3 position, float radius)
{
	for (auto plane : planes)
	{
		float dist = plane.x * position.x + plane.y * position.y + plane.z * position.z + plane.w - radius;
		if (dist > 0) return false;
	}
	return true;
}