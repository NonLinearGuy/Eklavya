#include"Engine.h"



int main()
{
	GLFWGame* engine = new Engine();
	if (engine->Initialize("HipHop v1.0", 1920, 1080, false))
		engine->Start();
	//engine->Destroy();
	delete engine;
	return 0;
}