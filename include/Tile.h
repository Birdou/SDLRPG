
#ifndef TILECOMPONENT_H
#define TILECOMPONENT_H

#include <SDL.h>

#include "Game.h"
#include "TextureManager.h"

class Tile
{
public:

	SDL_Texture * texture;
	SDL_Rect srcRect, destRect;
	Vector2D position;

	Tile() = default;

	~Tile()
	{
		SDL_DestroyTexture(texture);
	}

	Tile(int srcX, int srcY, int xpos, int ypos, int tsize, int tscale, std::string id)
	{
		texture = Game::assets->GetTexture(id);

		position.x = xpos;
		position.y = ypos;

		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = srcRect.h = tsize;

		destRect.x = static_cast<float>(xpos);
		destRect.y = static_cast<float>(ypos);
		destRect.w = destRect.h = tsize * tscale;
	}

	void update()
	{
		destRect.x = static_cast<int>(position.x - Game::camera.x);
		destRect.y = static_cast<int>(position.y - Game::camera.y);
	}

	void draw()
	{
		TextureManager::Draw(texture, srcRect, destRect);
	}

};

#endif