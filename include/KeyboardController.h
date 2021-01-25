
#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H

#include <cstring>

#include "SpriteComponent.h"
#include "Inventory.h"

class KeyboardController : public Component
{
public:
	TransformComponent * transform;
	SpriteComponent * sprite;

	bool released_escape = true;
	bool rl_lb = true;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override
	{
		if(Game::event.type == SDL_KEYDOWN)
		{
			switch(Game::event.key.keysym.sym)
			{
				case SDLK_w:
					transform->velocity.y = -1;
					sprite->Play("Walk");
					break;
				case SDLK_a:
					transform->velocity.x = -1;
					sprite->Play("Walk");
					sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
					break;
				case SDLK_s:
					transform->velocity.y = 1;
					sprite->Play("Walk");
					break;
				case SDLK_d:
					transform->velocity.x = 1;
					sprite->Play("Walk");
					break;
				case SDLK_ESCAPE:
					if(released_escape)
					{
						pauseMenu();
						released_escape = false;
					}
					break;
				default:
					break;
			}
		}
		if(Game::event.type == SDL_KEYUP)
		{
			switch(Game::event.key.keysym.sym)
			{
				case SDLK_w:
					transform->velocity.y = 0;
					break;
				case SDLK_a:
					transform->velocity.x = 0;
					sprite->spriteFlip = SDL_FLIP_NONE;
					break;
				case SDLK_s:
					transform->velocity.y = 0;
					break;
				case SDLK_d:
					transform->velocity.x = 0;
					break;
				case SDLK_ESCAPE:
					released_escape = true;
					break;
				default:
					break;
			}
			if(transform->velocity == Vector2D(0, 0))
			{
				sprite->Play("Idle");
			}
		}
	
		if(!rl_lb)
		{
			entity->getComponent<Inventory>().set[Inventory::weapon].item.useItem(entity);
		}
		else if(Game::event.type == SDL_MOUSEBUTTONDOWN)
		{
			
			switch(Game::event.button.button)
			{
				case SDL_BUTTON_LEFT:
					entity->getComponent<Inventory>().set[Inventory::weapon].item.useItem(entity);
					rl_lb = false;
					break;
				default:
					break;
			}
		}
		if(Game::event.type == SDL_MOUSEBUTTONUP)
		{
			switch(Game::event.button.button)
			{
				case SDL_BUTTON_LEFT:
					rl_lb = true;
					break;
				default:
					break;
			}
		}
	}
};

#endif