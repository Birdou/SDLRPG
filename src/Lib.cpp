
#include "Lib.h"

SDL_Color Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_Color color = {r, g, b, a};
	return color;
}

InterfaceTexture::InterfaceTexture(const char * Itexture, std::string Ifont, SDL_Color Ifont_color) :
	interface_texture(Itexture), interface_font(Ifont), interface_font_color(Ifont_color)
{}

InterfaceTexture::~InterfaceTexture()
{}

void nullFunc()
{}

float module(Vector2D v)
{
	return sqrt(pow(v.x, 2) + pow(v.y, 2));
}

float prodesc(Vector2D v1, Vector2D v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y);
}

float deg(float rad)
{
	return rad * (180 / M_PI);
}

float Ang(Vector2D v1, Vector2D v2)
{
	Vector2D v1_ = v1, v2_ = v2;
	float angle;

	v2_.x -= v1_.x;
	v2_.y -= v1_.y;
	v1_.x = 2;
	v1_.y = 0;

	if (v2_.y < 0)
	{
		angle = (M_PI - acos(prodesc(v1_, v2_) / (module(v1_) * module(v2_)))) + M_PI;
	}
	else
	{
		angle = acos(prodesc(v1_, v2_) / (module(v1_) * module(v2_)));
	}
	return angle;
}

float mouseAng(Vector2D center)
{
	float ang;
	Vector2D mousePos;

	SDL_GetMouseState(&mousePos.Ix, &mousePos.Iy);
	mousePos.Assoc();

	mousePos.x -= (WINDOW_WIDTH / 2) + center.x;
	mousePos.y -= (WINDOW_HEIGHT / 2) + center.y;

	ang = Ang(mousePos, Vector2D(-2, 0));

	return deg(ang);
}

void toMouse(Vector2D * vector)
{
	float ang;
	Vector2D mousePos;

	SDL_GetMouseState(&mousePos.Ix, &mousePos.Iy);
	mousePos.Assoc();

	mousePos.x -= WINDOW_WIDTH / 2;
	mousePos.y -= WINDOW_HEIGHT / 2;

	if (mousePos.y < 0)
	{
		ang = Ang(mousePos, Vector2D(-2, 0)) + 3.1415;
	}
	else
	{
		ang = Ang(mousePos, Vector2D(2, 0));
	}

	vector->Rotate(ang);
}

int cstrcmp (char * string1, const char * string2)
{
	if (*string1 == '\0' && *string2 == '\0')
	{
		return 0;
	}
	else
	{
		if( (*string1 != '\0' && *string2 != '\0') && *string1 == *string2)
		{
			return cstrcmp (string1 + 1, string2 + 1);
		}
		else
		{
			if(abs (*string1 - *string2) == 32)
			{
				return cstrcmp (string1 + 1, string2 + 1);
			}
			else
			{
				return *string1 - *string2;
			}
		}
	}
}

Vector2D getMousePos()
{
	Vector2D mousePos;

	SDL_GetMouseState(&mousePos.Ix, &mousePos.Iy);
	mousePos.Assoc();

	return mousePos;
}
