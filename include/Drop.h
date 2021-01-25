
#ifndef DROP_H
#define DROP_H

#include <SDL.h>
#include <chrono>

#include "Game.h"
#include "ProjectileComponent.h"

class Drop : public Component
{
public:
	std::map<const char *, float> dropRate;

	Drop()
	{}
	
	~Drop()
	{}

	void init() override
	{
		for (auto item : Game::itemIDs)
		{
			dropRate.emplace(item, 0);
		}
	}

	void defineDrop(const char * id, float rate)
	{
		dropRate[id] = rate;
	}

	void spawnDrop()
	{
		srand(time(NULL));
		for(auto item : dropRate)
		{
			int in = 100;
			float drop = item.second;
			if (drop == 0) continue;

			while (drop < 1) { drop *= 10; in *= 10; }
			if (rand() % in <= drop)
			{
				Entity& projectile = Game::assets->CreateProjectile(entity->getComponent<TransformComponent>().center, Vector2D(0, 0), 999, 0, 32, 32, 1, 0, item.first, Game::groupItemProjectiles, false);
				projectile.getComponent<ProjectileComponent>().IID = item.first;
				projectile.update();
			}
		}
	}
};

#endif
