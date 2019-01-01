#include "PawnActor.h"
#include <GLFW/glfw3.h>
#include "Transform.h"
#include "../Helpers.h"
#include "../Timer.h"

PawnActor::PawnActor(const std::string & name, ActorID id)
	:
	GameActor(name,id),
	UserInputListener()
{
}

PawnActor::~PawnActor()
{
}

void PawnActor::PollKeyAction()
{
	
}
