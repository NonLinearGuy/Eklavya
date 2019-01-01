#pragma once

#include "GameActor.h"
#include "../UserInputListener.h"

class PawnActor : public GameActor, public UserInputListener
{
public:
	PawnActor(const std::string& name,ActorID id);
	~PawnActor();

	void PollKeyAction()override;
};
