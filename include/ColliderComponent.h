
#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <SDL.h>

#include "CharacterComponent.h"

class Collision
{
public:
	static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB)
	{
		if (
			recA.x + recA.w >= recB.x &&
			recB.x + recB.w >= recA.x &&
			recA.y + recA.h >= recB.y &&
			recB.y + recB.h >= recA.y
			)
		{
			return true;
		}
		return false;
	}
};

class ColliderComponent : public Component
{
public:
	SDL_Rect collider, fit, hitBox;
	std::string tag;

	SDL_Texture * boxtex, * fittex, * hittex;
	SDL_Rect scrR, destR;

	int fitx = 0, fity = 0, fitw = 0, fith = 0;
	int hitx = 0, hity = 0, hitw = 0, hith = 0;

	TransformComponent * transform;
	
	ColliderComponent()
	{}
	
	ColliderComponent(std::string t)
	{
		tag = t;
	}
	
	ColliderComponent(std::string t, int xpos, int ypos, int w, int h)
	{
		tag = t;
		collider.x = fit.x = hitBox.x = xpos;
		collider.y = fit.y = hitBox.y = ypos;
		collider.h = fit.h = hitBox.h = h;
		collider.w = fit.w = hitBox.w = w;
	}

	void init() override
	{
		if(!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();

		boxtex = Game::assets->GetTexture("frame");
		fittex = Game::assets->GetTexture("fit");
		hittex = Game::assets->GetTexture("hitbox");

		scrR = { 0, 0, 32, 32 };
		destR = { collider.x, collider.y, collider.w, collider.h };

	}

	void Fit(int x, int y, int w, int h)
	{
		fitx = x;
		fity = y;
		fitw = w;
		fith = h;
	}

	void Hit(int x, int y, int w, int h)
	{
		hitx = x;
		hity = y;
		hitw = w;
		hith = h;
	}

	void update() override
	{
		int x = static_cast<int>(transform->position.x);
		int y = static_cast<int>(transform->position.y);

		if (tag != "terrain")
		{
			collider.x = x;
			collider.y = y;
			collider.w = transform->width * transform->scale;
			collider.h = transform->height * transform->scale;

			if(hitw == 0 || hith == 0)
			{
				hitBox = collider;
			}
			else
			{
				hitBox.x = hitx + x;
				hitBox.y = hity + y;
				hitBox.w = hitw;
				hitBox.h = hith;
			}
			if(fitw == 0 || fith == 0)
			{
				fit = collider;
			}
			else
			{
				fit.x = fitx + x;
				fit.y = fity + y;
				fit.w = fitw;
				fit.h = fith;
			}
		}

		destR.x = collider.x - Game::camera.x;
		destR.y = collider.y - Game::camera.y;
	}

	void draw() override
	{
		#ifdef DEBUG
			SDL_Rect tmp1 = {collider.x - Game::camera.x, collider.y - Game::camera.y, collider.w, collider.h};
			SDL_Rect tmp2 = {hitBox.x - Game::camera.x, hitBox.y - Game::camera.y, hitBox.w, hitBox.h};
			SDL_Rect tmp3 = {fit.x - Game::camera.x, fit.y - Game::camera.y, fit.w, fit.h};

			SDL_RenderCopy(Game::renderer, boxtex, &scrR, &tmp1);
			if(entity->hasComponent<CharacterComponent>())
			{
				
				SDL_RenderCopy(Game::renderer, hittex, &scrR, &tmp2);
				SDL_RenderCopy(Game::renderer, fittex, &scrR, &tmp3);
			}
		#endif
	}
};

#endif