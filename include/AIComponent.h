
#ifndef AICOMPONENT_H
#define AICOMPONENT_H

#include <vector>
#include <map>

#include "SpriteComponent.h"
#include "ColliderComponent.h"
#include "ProjectileComponent.h"
#include "UILabel.h"

class AIComponent : public Component
{
public:
	TransformComponent * transform;
	SpriteComponent * sprite;
	ColliderComponent * collider;

	AIComponent()
	{}

	AIComponent(int ai) : id(ai)
	{}

	~AIComponent()
	{}

	enum aiLabels
	{
		enemy,
		ally,
		soul,
		rat,
		damageLabel
	};

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
		collider = &entity->getComponent<ColliderComponent>();
	}

	void update() override
	{
		auto& colliders(getGroup(Game::groupColliders));
		
		switch(id)
		{

			case enemy:
			{
				sprite->speed = 75;
				Vector2D vel(0.75, 0);
				SDL_Rect view = {static_cast<int>(transform->center.x) - 256, static_cast<int>(transform->center.y) - 256, 512, 512};
				SDL_Rect area =  {static_cast<int>(transform->center.x) - 128, static_cast<int>(transform->center.y) - 128, 256, 256};
				if((sprite->current_frame == 0 || sprite->current_frame == 4) &&
					sprite->animIndex == 1)
				{
					playSound(Game::grass, 2, 0);
				}
				if (Collision::AABB(view, getPlayer().getComponent<ColliderComponent>().collider))
				{
					if(!Collision::AABB(area, getPlayer().getComponent<ColliderComponent>().collider))
					{
						float ang = Ang(transform->center, getPlayer().getComponent<TransformComponent>().center);
						for (auto& c : colliders)
						{
							SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
							if(Collision::AABB(cCol, collider->fit))
							{
								transform->position = transform->last_position;
								if(ang >= 314 && ang < 45) ang = 0.0f;
								else if(ang >= 45 && ang < 135) ang = 90.0f;
								else if(ang >= 135 && ang < 225) ang = 180.0f;
								else if(ang >= 225 && ang < 314) ang = 270.0f;
							}
						}
						vel.Rotate(ang);
						transform->velocity = vel;
						if (animation != 1)
						{
							sprite->Play(1, 8, 100);
							if(transform->velocity.x < 0)
							{
								sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
							}
							else
							{
								sprite->spriteFlip = SDL_FLIP_NONE;
							}
						}
					}
					else
					{
						transform->velocity.Zero();
						if (SDL_GetTicks() - lastHit > 1000 / 3)
						{
							Game::assets->CreateProjectile(transform->center, getPlayer().getComponent<TransformComponent>().center, 200, 2, 32, 32, 1, 0, "projectile", Game::groupEnemyProjectiles, true);
							playSound(Game::woosh_f1, 3, 0);
							lastHit = SDL_GetTicks();
						}
						if (animation != 0)
						{
							sprite->Play(0, 3, 100);
							animation = 0;
						}
					}
				}
				else
				{
					transform->velocity.Zero();
					if (animation != 0)
					{
						sprite->Play(0, 3, 100);
						animation = 0;
					}
				}
				break;
			}

			case ally:
			{
				sprite->speed = 100;
				Vector2D vel(1, 0);
				int view = 256;
				SDL_Rect area =  {static_cast<int>(transform->center.x) - 64, static_cast<int>(transform->center.y) - 64, 128, 128};
				if((sprite->current_frame == 0 || sprite->current_frame == 4) &&
					sprite->animIndex == 1)
				{
					playSound(Game::grass, 2, 0);
				}
				if(!Collision::AABB(area, getPlayer().getComponent<ColliderComponent>().collider))
				{
					vel.Rotate(Ang(transform->center, getPlayer().getComponent<TransformComponent>().center));
					transform->velocity = vel;
					if (animation != 1)
					{
						sprite->Play(1, 8, 100);
						animation = 1;
						if(transform->velocity.x < 0)
						{
							sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
						}
						else
						{
							sprite->spriteFlip = SDL_FLIP_NONE;
						}
					}
				}
				else
				{
					transform->velocity.Zero();
					if (SDL_GetTicks() - lastHit > 1000 / 3)
					{
						Vector2D target = NearEnemy(transform->center, view);
						if(!(target == Vector2D(-1, -1)))
						{
							Game::assets->CreateProjectile(transform->center, target, 200, 2, 32, 32, 1, 0, "friendly_projectile", Game::groupFriendlyProjectiles, false);
							playSound(Game::woosh_f1, 3, 0);
							lastHit = SDL_GetTicks();
						}
					}
					if (animation != 0)
					{
						sprite->Play(0, 3, 100);
						animation = 0;
					}
				}
				break;
			}

			case soul:
			{
				Vector2D vel(0.8, 0);
				float angle = Ang(transform->center, getPlayer().getComponent<TransformComponent>().center);
				vel.Rotate(angle);
				transform->velocity = vel;
				sprite->angle = deg(angle);
				break;
			}

			case rat:
			{
				sprite->speed = 75;
				Vector2D vel(0.75, 0);
				SDL_Rect view = {static_cast<int>(transform->center.x) - 256, static_cast<int>(transform->center.y) - 256, 512, 512};
				if((sprite->current_frame == 0 || sprite->current_frame == 4) &&
					sprite->animIndex == 1)
				{
					playSound(Game::grass, 2, 0);
				}
				float ang = Ang(transform->center, getPlayer().getComponent<TransformComponent>().center);
				if (Collision::AABB(view, getPlayer().getComponent<ColliderComponent>().collider))
				{
					if(!Collision::AABB(collider->collider, getPlayer().getComponent<ColliderComponent>().hitBox))
					{
						
						for (auto& c : colliders)
						{
							SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
							if(Collision::AABB(cCol, collider->fit))
							{
								transform->position = transform->last_position;
								if(ang >= 314 && ang < 45) ang = 0.0f;
								else if(ang >= 45 && ang < 135) ang = 90.0f;
								else if(ang >= 135 && ang < 225) ang = 180.0f;
								else if(ang >= 225 && ang < 314) ang = 270.0f;
							}
						}
						vel.Rotate(ang);
						transform->velocity = vel;
						if (animation != 1)
						{
							sprite->Play(1, 3, 100);
							animation = 1;
							if(transform->velocity.x < 0)
							{
								sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
							}
							else
							{
								sprite->spriteFlip = SDL_FLIP_NONE;
							}
						}
					}
					else
					{
						transform->velocity.Zero();
						if (SDL_GetTicks() - lastHit > 1000 / 3)
						{
							auto& projectile = Game::assets->CreateProjectile(transform->center, getPlayer().getComponent<TransformComponent>().center, 999, 0, 32, 32, 5, 0, "attack", Game::groupEnemyProjectiles, true);
							projectile.getComponent<ProjectileComponent>().damage = entity->getComponent<CharacterComponent>().Damage();
							projectile.getComponent<SpriteComponent>().destroyAfter = true;
							projectile.getComponent<SpriteComponent>().Play(0, 5, 100);
							if(deg(ang) >= 90 && deg(ang) < 270)
							{
								projectile.getComponent<SpriteComponent>().spriteFlip = SDL_FLIP_VERTICAL;
								sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
							}
							else
							{
								sprite->spriteFlip = SDL_FLIP_NONE;
							}
							playSound(Game::woosh_f1, 3, 0);
							lastHit = SDL_GetTicks();
						}
						if (animation != 0)
						{
							sprite->Play(0, 3, 100);
							animation = 0;
						}
					}
				}
				else
				{
					transform->velocity.Zero();
					if (animation != 0)
					{
						sprite->Play(0, 3, 100);
						animation = 0;
					}
				}
				break;
			}

			case damageLabel:
			{
				entity->getComponent<UILabel>().position.y -= 1;
				entity->getComponent<UILabel>().alpha -= 2;
				if(entity->getComponent<UILabel>().alpha <= 0)
				{
					entity->destroy();
				}
				entity->getComponent<UILabel>().SetAlpha();
				break;
			}

			default:
			{
				break;
			}

		}
	}

private:
	int id;
	int animation = 0;
	int lastHit = 0;
};

#endif