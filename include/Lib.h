
#ifndef LIB_H
#define LIB_H

#include <SDL.h>
#include <string>

#include "Vector2D.h"
#include "ECS.h"

#define WINDOW_HEIGHT 640
#define WINDOW_WIDTH 800

#ifdef DEBUG
#define Debug(x) x
#else
#define Debug(x)
#endif

SDL_Color Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

class InterfaceTexture
{
public:
	const char * interface_texture;
	std::string interface_font;
	SDL_Color interface_font_color;

	InterfaceTexture(const char * interface_texture, std::string interface_font, SDL_Color interface_font_color);
	~InterfaceTexture();
};

typedef Entity& (*EnemyPrototype)(Vector2D, int);

void nullFunc();

float module(Vector2D v1);

float prodesc(Vector2D v1, Vector2D v2);

float deg(float rad);

float Ang(Vector2D v1, Vector2D v2);

float mouseAng(Vector2D vector);

void toMouse(Vector2D * vector);

int cstrcmp (char * string1, const char * string2);

Vector2D getMousePos();

#endif