
#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Game.h"
#include "TransformComponent.h"

class ProjectileComponent : public Component
{
public:
	const char * IID;
	int damage = 1;

	int range = 0;
	int speed = 0;
	int distance = 0;

	ProjectileComponent(int rng, int sp, Vector2D vel) : range(rng), speed(sp), velocity(vel)
	{}
	~ProjectileComponent()
	{}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocity;
	}

	void update() override
	{
		distance += speed;

		if (distance > range)
		{
			entity->destroy();
		}
		else if (transform->position.x > Game::camera.x + Game::camera.w ||
				transform->position.x < Game::camera.x ||
				transform->position.y > Game::camera.y + Game::camera.h ||
				transform->position.y < Game::camera.y)
		{
			entity->destroy();
		}
	}

private:
	TransformComponent * transform;
	Vector2D velocity;
};

#endif