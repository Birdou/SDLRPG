
#ifndef COLLISION_H
#define COLLISION_H

#include <SDL.h>

#include "ColliderComponent.h"

class Collision
{
public:
	static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB);
	static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);
};

#endif