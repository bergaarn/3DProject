// Includes
#include "Engine/Engine.h"

int main()
{
	#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	Engine engine;
	engine.run();

	return 0;
}