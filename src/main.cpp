
#include "Game.h"

Game * game = nullptr;
bool fullscreen = false, debugging = false;

int main (int argc, char* argv[])
{
	system("chcp 65001 >nul");
	setlocale(LC_ALL, "pt_BR_utf8");

	int FPS = 60;

	for (int args = 1; args < argc; args++)
	{
		if (cstrcmp(argv[args], "-FPS") == 0)
		{
			if (++args < argc)
			{
				FPS = atoi(argv[args]);
				if (FPS < 1)
				{
					FPS = 1;
				}
			}
			else
			{
				std::cout << "Um valor deve ser fornecido para a opção \"" << argv[args - 1] << "\"" << std::endl;
				return -1;
			}
		}
		else if (cstrcmp(argv[args], "-FULLSCREEN") == 0 || cstrcmp(argv[args], "--FS") == 0)
		{
			fullscreen = true;
		}
		else if (cstrcmp(argv[args], "-DEBUG") == 0 || cstrcmp(argv[args], "--DB") == 0)
		{
			debugging = true;
		}
		else
		{
			std::cout << "Opção inválida \"" << argv[args] << "\"" << std::endl;
			return -1;
		}
	}

	if (!debugging)
	{
		//DISABLE MESSAGES
		fclose(stdout);
	}

	const int frameDelay = 1000 / FPS;
	Uint32 frameStart;
	int frameTime;

	game = new Game();

	game->init("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, fullscreen);

	while(game->running())
	{
		frameStart = SDL_GetTicks();

		game->handleEvents();
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

	game->clean();

	return 0;
}