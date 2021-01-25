
#ifndef BUTTONTEXTURE_H
#define BUTTONTEXTURE_H

#include <SDL.h>
#include <string>

class ButtonTexture
{
public:
	const char * button = "";
	const char * button_hovered = "";
	const char * button_pressed = "";
	std::string button_font = "";
	SDL_Color button_font_color;

	ButtonTexture();
	ButtonTexture(const char * btn);
	ButtonTexture(const char * btn, std::string font, SDL_Color colour);
	ButtonTexture(const char * btn, const char * btnHovered, const char * btnPressed, std::string font, SDL_Color colour);
	~ButtonTexture();
};


#endif