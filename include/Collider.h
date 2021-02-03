
#ifndef COLLIDER_H
#define COLLIDER_H

#include <SDL.h>

#include "Game.h"

class Collider
{
public:
	SDL_Rect collider;
	SDL_Texture * boxtex;
	SDL_Rect destR;

	Collider()
	{
		boxtex = Game::assets->GetTexture("frame");
		destR = { collider.x, collider.y, collider.w, collider.h };
	}
	
	Collider(int xpos, int ypos, int w, int h)
	{
		collider = { xpos, ypos, w, h };
		boxtex = Game::assets->GetTexture("frame");
		destR = { collider.x, collider.y, collider.w, collider.h };
	}

	void update()
	{
		destR.x = collider.x - Game::camera.x;
		destR.y = collider.y - Game::camera.y;
	}

	void draw()
	{
		#ifdef DEBUG
			SDL_Rect tmp = {collider.x - Game::camera.x, collider.y - Game::camera.y, collider.w, collider.h};
			TextureManager::Draw(boxtex, tmp);
		#endif
	}
};

#endif