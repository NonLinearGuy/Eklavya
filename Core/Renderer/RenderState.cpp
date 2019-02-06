#include"RenderState.h"
#include<glad/glad.h>


//blend state

void AlphaRenderState::Enter()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

void AlphaRenderState::Exit()
{
	glDisable(GL_BLEND);
}


//skybox


void SkyBoxState::Enter()
{
	glDepthFunc(GL_LEQUAL);
}


void SkyBoxState::Exit()
{
	
	glDepthFunc(GL_LESS);
}

//defualt 

void DefaultState::Enter()
{
	
}


void DefaultState::Exit()
{

}


//UI

void UIState::Enter()
{
	if (GL_FALSE == glIsEnabled(GL_BLEND))
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	}
}

void UIState::Exit()
{
	glDisable(GL_BLEND);
}