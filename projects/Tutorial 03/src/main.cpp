#include "Game.h"
#include "Logging.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int main() {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	long long memBreak = 0;
	if (memBreak) _CrtSetBreakAlloc(memBreak);

	{
		Logger::Init();

		Game* game = new Game();
		game->Run();
		delete game;

		Logger::Uninitialize();
	}

	//_CrtDumpMemoryLeaks();

	return 0;
}

