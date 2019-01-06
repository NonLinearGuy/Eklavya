#include"Engine.h"



int main()
{
	GLFWGame* engine = new Engine();
	if (engine->Initialize("HipHop v1.0", 1366, 768, true))
		engine->Start();
	//engine->Destroy();
	delete engine;
	return 0;
}