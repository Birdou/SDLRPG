
#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>

#include "Game.h"

class Button : public Component
{
public:
	bool visible = false;
	SDL_Rect btn;
	ButtonTexture buttonTexture;
	SDL_Texture * fnt, * tex;
	SDL_Texture * bTex, * bHov, * bPre;
	std::string buttonText;
	SDL_Rect srcR, destR;
	bool pressed = false;
	int buttonPadding = 2;
	int scale = 1;
	Function function;

	Button()
	{}

	Button(int xpos, int ypos, int width, int height)
	{
		srcR.x = 0;
		srcR.y = 0;
		srcR.h = srcR.w = 32;

		destR.w = width;
		destR.h = height;
		destR.x = xpos - (destR.w / 2);
		destR.y = ypos - (destR.h / 2);

	}

	Button(int xpos, int ypos, int width, int height, int padding, int sc, ButtonTexture tex, std::string text, bool vis, Function func) :
		visible(vis), buttonTexture(tex), buttonText(text), buttonPadding(padding), scale(sc), function(func)
	{
		srcR.x = 0;
		srcR.y = 0;
		srcR.h = srcR.w = 32;

		destR.w = width * sc;
		destR.h = height * sc;
		destR.x = xpos - (destR.w / 2);
		destR.y = ypos - (destR.h / 2);

		this->tex = Game::assets->GetTexture(buttonTexture.button);
		this->bTex = Game::assets->GetTexture(buttonTexture.button);
		this->bHov = Game::assets->GetTexture(buttonTexture.button_hovered);
		this->bPre = Game::assets->GetTexture(buttonTexture.button_pressed);

		SetButtonText(text);
	}

	~Button()
	{
		SDL_DestroyTexture(fnt);
		SDL_DestroyTexture(tex);

		SDL_DestroyTexture(bTex);
		SDL_DestroyTexture(bHov);
		SDL_DestroyTexture(bPre);
	}

	void SetButtonText(std::string text)
	{
		if(buttonTexture.button_font == "") return;
		SDL_Surface * surf = TTF_RenderText_Blended(Game::assets->GetFont(buttonTexture.button_font), text.c_str(), buttonTexture.button_font_color);
		fnt = SDL_CreateTextureFromSurface(Game::renderer, surf);
		SDL_FreeSurface(surf);
		SDL_QueryTexture(fnt, nullptr, nullptr, &btn.w, &btn.h);
		btn.w += buttonPadding * 2;
		btn.h += buttonPadding * 2;
		btn.x = destR.x + ((destR.w - btn.w) / 2);
		btn.y = destR.y + ((destR.h - btn.h) / 2);
	}

	void update() override
	{
		if (visible)
		{
			Vector2D mouse = getMousePos();
			if(mouse.x < destR.x + destR.w && mouse.y < destR.y + destR.h &&
				mouse.x >= destR.x && mouse.y >= destR.y)
			{
				if (!pressed)
				{
					tex = bHov;
					if(Game::event.type == SDL_MOUSEBUTTONDOWN)
					{
						switch(Game::event.button.button)
						{
							case SDL_BUTTON_LEFT:
								pressed = true;
								tex = bPre;
								function();
								break;
							default:
								break;
						}
					}
				}
				if(Game::event.type == SDL_MOUSEBUTTONUP)
				{
					switch(Game::event.button.button)
					{
						case SDL_BUTTON_LEFT:
							pressed = false;
							tex = bHov;
							break;
						default:
							break;
					}
				}
			}
			else
			{
				tex = bTex;
			}
		}
	}

	void draw() override
	{
		if(visible)
		{
			//TextureManager::Draw(tex, srcR, destR, 0, SDL_FLIP_NONE);
			SDL_RenderCopy(Game::renderer, tex, nullptr, &destR);
			SDL_RenderCopy(Game::renderer, fnt, nullptr, &btn);
		}
	}
};



#endif