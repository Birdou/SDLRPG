
#ifndef ITEMCOMPONENT_H
#define ITEMCOMPONENT_H

#include <SDL.h>
#include <string>

#include "SpriteComponent.h"
#include "CharacterComponent.h"
#include "ProjectileComponent.h"

class ItemComponent : public Component
{
public:
	std::string ID;
	std::string ItemName;
	int style = -1;
	int modifier = 0;

	int damage = 0;
	int last_use = 0;

	ItemComponent()
	{}

	ItemComponent(std::string id, std::string name, int st) :
		ID(id), ItemName(name), style(st)
	{}

	~ItemComponent()
	{}

	enum styles
	{
		sword,
		staff,
		bow,
	};

	enum modifiers
	{};

	void useItem(Entity * owner)
	{
		if (SDL_GetTicks() - last_use > 1000 / 3)
		{
			last_use = SDL_GetTicks();
		}
		else
		{
			return;
		}

		Vector2D mouse;
		SDL_GetMouseState(&mouse.Ix, &mouse.Iy);
		mouse.Assoc();
		mouse.x += Game::camera.x;
		mouse.y += Game::camera.y;

		switch(style)
		{
			case sword:
			{
				auto& projectile = Game::assets->CreateProjectile(owner->getComponent<TransformComponent>().center, mouse, 999, 0, 32, 32, 6, 0, "melee", Game::groupFriendlyProjectiles, true);
				projectile.getComponent<ProjectileComponent>().damage = owner->getComponent<CharacterComponent>().strength * damage * 0.75;
				projectile.getComponent<SpriteComponent>().destroyAfter = true;
				projectile.getComponent<SpriteComponent>().Play(0, 5, 100);
				break;
			}
			case staff:
			{
				auto& projectile = Game::assets->CreateProjectile(owner->getComponent<TransformComponent>().center, mouse, 200, 2, 32, 32, 1, 0, "mage", Game::groupFriendlyProjectiles, true);
				projectile.getComponent<ProjectileComponent>().damage = owner->getComponent<CharacterComponent>().intelligence * damage * 0.75;
				break;
			}
			case bow:
			{
				auto& projectile = Game::assets->CreateProjectile(owner->getComponent<TransformComponent>().center, mouse, 200, 3, 32, 32, 1, 0, "ranged", Game::groupFriendlyProjectiles, true);
				projectile.getComponent<ProjectileComponent>().damage = owner->getComponent<CharacterComponent>().dexterity * damage * 0.75;
				break;
			}
			default:
			{
				auto& projectile = Game::assets->CreateProjectile(owner->getComponent<TransformComponent>().center, mouse, 999, 0, 32, 32, 5, 0, "melee", Game::groupFriendlyProjectiles, true);
				projectile.getComponent<ProjectileComponent>().damage = (owner->getComponent<CharacterComponent>().strength + owner->getComponent<CharacterComponent>().intelligence + owner->getComponent<CharacterComponent>().dexterity) / 3;
				projectile.getComponent<SpriteComponent>().destroyAfter = true;
				projectile.getComponent<SpriteComponent>().Play(0, 5, 100);
				break;
			}
		}
	}
};

#endif