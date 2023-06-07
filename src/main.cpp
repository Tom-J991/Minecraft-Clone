#include "Game.h"
#include "Common.h"

#ifndef MC_DIST
int main()
#else
int WinMain()
#endif
{
	int error = 0;

#ifdef MC_DEBUG
	spdlog::set_level(spdlog::level::trace);
#else
	spdlog::set_level(spdlog::level::info);
#endif

	Game *game = Game::Get();
	try
	{
		game->Run();
	}
	catch (const std::exception &e)
	{
		spdlog::error("Exception: {0}", e.what());
		error = 1;
	}
	delete game;
	game = nullptr;

	return error;
}
