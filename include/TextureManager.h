
#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SDL.h>

#include "Game.h"

struct TextureManager
{
	static SDL_Texture* LoadTexture(const char * fileName);
	static void Draw(SDL_Texture* tex, const SDL_Rect& dest);
	static void Draw(SDL_Texture* tex, const SDL_Rect& src, const SDL_Rect& dest);
	static void Draw(SDL_Texture* tex, const SDL_Rect& src, const SDL_Rect& dest, float ang, SDL_RendererFlip flip);
};

#endif