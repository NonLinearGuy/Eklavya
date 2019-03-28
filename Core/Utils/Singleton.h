#pragma once

template<typename Type>
class Singleton
{
public:
	static Type& GetInstance()
	{
		static Type instance = Type();
		return instance;
	}

protected:
	Singleton() {}
};