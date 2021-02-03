
#ifndef SPAWNCONTROLLER_H
#define SPAWNCONTROLLER_H

#include <SDL.h>

#include "SpriteComponent.h"
#include "ColliderComponent.h"

class Spawn
{
public:
	Vector2D anchor;
	int range, last = 0;
	size_t max_members;
	Uint8 frequency;
	EnemyPrototype function;
	int group, effective_range;
	SDL_Texture * effc, * rang;
	int positions;
	std::vector<Vector2D> activePositions;

	Spawn(int xpos, int ypos, int rng, int mMembers, int freq, EnemyPrototype func, int group) :
		range(rng), max_members(mMembers), frequency(freq), function(func), group(group)
	{
		anchor.x = xpos;
		anchor.y = ypos;
		effective_range = range * 0.75;

		effc = Game::assets->GetTexture("spawneffectframe");
		rang = Game::assets->GetTexture("spawnframe");

		int resolution = 16;
		int vectorside = ((effective_range) * 2) / resolution;
		positions = pow(vectorside, 2);

		for(int i = 0; i < vectorside; i++)
		{
			int y = ((ypos - effective_range) + (i * resolution));
			for(int j = 0; j < vectorside; j++)
			{
				int x = ((xpos - effective_range) + (j * resolution));
				activePositions.emplace_back(Vector2D(x, y));
			}
		}
		printf("ACTIVE POSITIONS: %i\n", positions); 
	}

	~Spawn()
	{
		SDL_DestroyTexture(effc);
		SDL_DestroyTexture(rang);
	}

	void update()
	{
		auto& components = getGroup(group);
		for(auto& c : components)
		{
			if(c->getComponent<TransformComponent>().center.x > anchor.x + range ||
				c->getComponent<TransformComponent>().center.y > anchor.y + range ||
				c->getComponent<TransformComponent>().center.x < anchor.x - range ||
				c->getComponent<TransformComponent>().center.y < anchor.y - range)
			{
				//c->getComponent<SpriteComponent>().Play("puff");
				c->getComponent<SpriteComponent>().current_frame = 0;
				c->getComponent<SpriteComponent>().destroyAfter = true;
				c->getComponent<SpriteComponent>().animIndex = 2;
				c->getComponent<SpriteComponent>().frames = 8;
				c->getComponent<SpriteComponent>().speed = 100;
			}
		}

		if(SDL_GetTicks() - last > 10000 && components.size() < max_members)
		{
			if(positions <= 0)
			{
				printf("EXCEPTION HANDLED: positions == %i\n", positions);
				return;
			}
			int pos = rand() % positions;

			Entity& entity = function(activePositions[pos], group);


			auto& colliders(getGroup(Game::groupColliders));
			SDL_Rect eCol = entity.getComponent<ColliderComponent>().fit;
			
			for (auto& c : colliders)
			{
				SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
				if(Collision::AABB(cCol, eCol))
				{
					entity.destroy();
					printf("POSITION DEACTIVED: (%.2lf,%.2lf)\n", activePositions[pos].x, activePositions[pos].y);
					activePositions.erase(activePositions.begin() + pos);
					positions--;
					return;
				}
			}
			entity.getComponent<SpriteComponent>().visible = true;
			last = SDL_GetTicks();
		}
	}

	void draw()
	{
		#ifdef DEBUG
			int x = anchor.x - effective_range - Game::camera.x;
			int y = anchor.y - effective_range - Game::camera.y;
			int w = effective_range * 2;
			SDL_Rect effcR = {x, y, w, w};
			x = anchor.x - range - Game::camera.x;
			y = anchor.y - range - Game::camera.y;
			w = range * 2;
			SDL_Rect rangR = {x, y, w, w};
			TextureManager::Draw(effc, effcR);
			TextureManager::Draw(rang, rangR);
		#endif
	}
};

#endif