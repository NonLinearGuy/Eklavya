#include"RenderState.h"
#include<glad/glad.h>


//blend state
void BlendState::Enter()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

void BlendState::Exit()
{
	glDisable(GL_BLEND);
}


//skybox

void SkyBoxState::Enter()
{
	if(glIsEnabled(GL_DEPTH_TEST))
		glDepthFunc(GL_LEQUAL);
}


void SkyBoxState::Exit()
{
	if(glIsEnabled(GL_DEPTH_TEST))
		glDepthFunc(GL_LESS);
}

//defualt 

void DefaultState::Enter()
{
	
}


void DefaultState::Exit()
{

}
