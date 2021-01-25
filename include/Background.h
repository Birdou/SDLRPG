
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <string>

#include "Game.h"

class Background : public Component
{
private:
	SDL_Rect background;
	SDL_Texture * texture;

public:
	bool visible = false;

	Background(int xpos, int ypos, int width, int height, std::string tex)
	{
		background.w = width;
		background.h = height;
		background.x = xpos /*- (background.w / 2)*/;
		background.y = ypos /*- (background.h / 2)*/;

		texture = Game::assets->GetTexture(tex);
	}

	~Background()
	{
		SDL_DestroyTexture(texture);
	}

	void draw() override
	{
		if(visible)
		{
			SDL_RenderCopy(Game::renderer, texture, nullptr, &background);
		}
	}
};

#endif