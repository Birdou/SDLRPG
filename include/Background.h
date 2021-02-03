
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <string>

#include "TextureManager.h"

class Background : public Component
{
private:
	SDL_Rect destRect;
	SDL_Texture* texture;

public:
	bool visible = false;

	Background(int xpos, int ypos, int width, int height, std::string tex)
	{
		destRect.w = width;
		destRect.h = height;
		destRect.x = xpos;
		destRect.y = ypos;

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
			TextureManager::Draw(texture, destRect);
		}
	}
};

#endif