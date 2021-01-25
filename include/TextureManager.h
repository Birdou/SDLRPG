
#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SDL.h>

#include "Game.h"

class TextureManager
{
public:
	static SDL_Texture * LoadTexture(const char * fileName);
	static void Draw(SDL_Texture * tex, SDL_Rect src, SDL_Rect dest, float ang, SDL_RendererFlip flip);
};

#endif