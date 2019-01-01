#include"Engine.h"

using namespace HipHop;

int main()
{
	GLFWGame* engine = new Engine();
	if (engine->Initialize("HipHop v1.0", 1024, 640, false))
		engine->Start();
	//engine->Destroy();
	delete engine;
	return 0;
}