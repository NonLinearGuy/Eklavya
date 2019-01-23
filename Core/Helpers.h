#pragma once

#include <memory>
#include<functional>
#include "Event/Events.h"

template<typename Type>
inline std::shared_ptr<Type> MakeSharedPtr(std::weak_ptr<Type> weakObject)
{
	if (!weakObject.expired())
		return std::shared_ptr<Type>(weakObject);
	else
		std::shared_ptr<Type>();
}


const int ACTOR_NOT_NEEDED = -1;
const int ACTOR_BOUNDING_VOLUME = -2;

