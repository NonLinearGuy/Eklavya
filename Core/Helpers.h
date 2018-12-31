#pragma once

#include <memory>

template<typename Type>
inline std::shared_ptr<Type> MakeSharedPtr(std::weak_ptr<Type> weakObject)
{
	if (!weakObject.expired())
		return std::shared_ptr<Type>(weakObject);
	else
		std::shared_ptr<Type>();
}

