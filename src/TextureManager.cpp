
#include "TextureManager.h"

#include <SDL_image.h>
#include <cstring>

SDL_Texture* TextureManager::LoadTexture(const char * texture)
{
	SDL_Surface* tmpSurface = IMG_Load(texture);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	if(strcmp(SDL_GetError(), "") != 0)
	{
		std::cout << "LOADTEXTURE:: " << texture << " : " << SDL_GetError() << std::endl;
		SDL_ClearError();
	}
	return tex;
}

void TextureManager::Draw(SDL_Texture* tex, const SDL_Rect& dest)
{
	if (tex == NULL)
	{
		std::cout << "DRAWFONT:: null texture" << std::endl;
		return;
	}
	SDL_RenderCopyEx(Game::renderer, tex, nullptr, &dest, 0, NULL, SDL_FLIP_NONE);
}

void TextureManager::Draw(SDL_Texture* tex, const SDL_Rect& src, const SDL_Rect& dest)
{
	if (tex == NULL)
	{
		std::cout << "DRAW:: null texture" << std::endl;
		return;
	}
	SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, 0, NULL, SDL_FLIP_NONE);
}

void TextureManager::Draw(SDL_Texture* tex, const SDL_Rect& src, const SDL_Rect& dest, float ang, SDL_RendererFlip flip)
{
	if (tex == NULL)
	{
		std::cout << "DRAW:: null texture" << std::endl;
		return;
	}
	SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, ang, NULL, flip);
}

