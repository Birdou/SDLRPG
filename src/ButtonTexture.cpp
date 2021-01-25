
#include "ButtonTexture.h"

ButtonTexture::ButtonTexture()
{}

ButtonTexture::ButtonTexture(const char * btn)
{
	button = button_hovered = button_pressed = btn;
}

ButtonTexture::ButtonTexture(const char * btn, std::string font, SDL_Color colour) :
	button_font(font), button_font_color(colour)
{
	button = button_hovered = button_pressed = btn;
}

ButtonTexture::ButtonTexture(const char * btn, const char * btnHovered, const char * btnPressed, std::string font, SDL_Color colour) :
	button(btn), button_hovered(btnHovered), button_pressed(btnPressed), button_font(font), button_font_color(colour)
{}

ButtonTexture::~ButtonTexture()
{}
