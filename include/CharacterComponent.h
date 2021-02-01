
#ifndef CHARACTERCOMPONENT_H
#define CHARACTERCOMPONENT_H

#include <SDL.h>

#include "Game.h"
#include "TextureManager.h"
#include "TransformComponent.h"

struct Attribute
{
	float value = 100;
	float max = 100;
	SDL_Texture * tex;
	SDL_Rect attbar;
};

class CharacterComponent : public Component
{
public:
	SDL_Rect srcR, bar;
	SDL_Texture * main_bar;
	Attribute life;
	TransformComponent * transform;

	int basedmg = 0, dmg = 1;

	int souls = 100;
	int level = 1;

	int constitution = 1, strength = 1, intelligence = 1, dexterity = 1;

	CharacterComponent()
	{}

	CharacterComponent(int basedmg, int dmg, int souls, int maxlife):
	basedmg(basedmg), dmg(dmg), souls(souls)
	{
		life.max = maxlife;
	}

	~CharacterComponent()
	{}

	int Damage()
	{
		return (rand() % dmg) + basedmg;
	}

	int conUpCost() { return (15 * constitution) + (10 * level); }
	int strUpCost() { return (15 * strength) + (10 * level); }
	int intUpCost() { return (15 * intelligence) + (10 * level); }
	int dexUpCost() { return (15 * dexterity) + (10 * level); }

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		life.value = life.max;

		if (entity->hasGroup(Game::groupPlayers))
		{
			life.tex = Game::assets->GetTexture("ally_life_bar");
		}
		else
		{
			life.tex = Game::assets->GetTexture("enemy_life_bar");
		}
		main_bar = Game::assets->GetTexture("life_bar_background");

		srcR.x = srcR.y = 0;
		srcR.w = srcR.h = 32;

		life.attbar.h = bar.h = 32;
		life.attbar.w = bar.w = 128;
		life.attbar.x = bar.x = transform->center.x - (bar.w / 2);
		life.attbar.y = bar.y = transform->center.y + bar.h;
	}

	void update() override
	{
		int x = static_cast<int>(transform->position.x);
		int y = static_cast<int>(transform->position.y) - 32; //32 = OFFSET

		if (life.value <= 0) life.value = life.max;

		life.attbar.w = static_cast<int>(bar.w * (life.value / life.max));
		life.attbar.x = bar.x = x - (bar.w / 2);
		life.attbar.y = bar.y = y + bar.h;

		life.attbar.x = bar.x = x - Game::camera.x;
		life.attbar.y = bar.y = y - Game::camera.y;
	}

	void draw() override
	{
		TextureManager::Draw(main_bar, srcR, bar, 0, SDL_FLIP_NONE);
		TextureManager::Draw(life.tex, srcR, life.attbar, 0, SDL_FLIP_NONE);
	}
};


#endif