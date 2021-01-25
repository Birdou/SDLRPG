
#include "TextureManager.h"

#include <SDL_image.h>
#include <cstring>

SDL_Texture * TextureManager::LoadTexture(const char * texture)
{
	SDL_Surface * tmpSurface = IMG_Load(texture);
	SDL_Texture * tex = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	if(strcmp(SDL_GetError(), "") != 0)
	{
		printf("LOADTEXTURE:: %s : %s\n", texture, SDL_GetError());
		SDL_ClearError();
	}
	return tex;
}

void TextureManager::Draw(SDL_Texture * tex, SDL_Rect src, SDL_Rect dest, float ang, SDL_RendererFlip flip)
{
	if (tex == NULL) return;
	SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, ang, NULL, flip);
}