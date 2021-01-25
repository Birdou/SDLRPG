
#ifndef UILABEL_H
#define UILABEL_H

#include <SDL.h>
#include <SDL_ttf.h>

#include "Game.h"

class UILabel : public Component
{
public:
	int x, y;
	double alpha = 255.0;
	bool visible = true, aligned_right = false, aligned_center = false;
	SDL_Rect position;
	std::string labelText;
	std::string labelFont;
	SDL_Color textColour;
	SDL_Texture * labelTexture = NULL;

	UILabel(int xpos, int ypos, std::string text, std::string font, SDL_Color colour) : 
		labelFont(font), textColour(colour)
	{
		x = position.x = xpos;
		y = position.y = ypos;

		SetLabelText(text);
	}
	
	~UILabel()
	{
		SDL_DestroyTexture(labelTexture);
	}

	void SetAlpha()
	{
		SDL_SetTextureAlphaMod(labelTexture, static_cast<int>(alpha));
	}

	void SetLabelText(std::string text)
	{
		if(visible)
		{
			if(labelText == text) return;
			if(labelTexture != NULL)
				SDL_DestroyTexture(labelTexture);
			SDL_Surface * surf = TTF_RenderText_Blended(Game::assets->GetFont(labelFont), text.c_str(), textColour);
			labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
			SDL_FreeSurface(surf);
			SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
			if(aligned_right)
			{
				position.x = x - position.w;
			}
			if(aligned_center)
			{
				position.x = x - (position.w / 2);
			}
			labelText = text;
		}
	}

	void draw() override
	{
		if(visible)
		{
			SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
		}
	}
};

#endif