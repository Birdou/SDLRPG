
#ifndef ENEMY_RAT_H
#define ENEMY_RAT_H

#include "ECS.h"
#include "AIComponent.h"
#include "Drop.h"

Entity& Rat(Vector2D position, int group)
{
	Entity& entity(getManager().addEntity());

	entity.addComponent<TransformComponent>(position.x, position.y, 32, 32, 4);
	entity.addComponent<SpriteComponent>("rat", true, 0, 0);
	entity.addComponent<ColliderComponent>("rat");
	entity.addComponent<AIComponent>(AIComponent::rat);
	entity.addComponent<CharacterComponent>(1, 2, 16, 50);
	entity.addComponent<Drop>();

	entity.getComponent<ColliderComponent>().Fit(32, 68, 73, 18);
	entity.getComponent<ColliderComponent>().Hit(34, 40, 81, 41);

	entity.getComponent<Drop>().defineDrop("sword", 15);
	entity.getComponent<Drop>().defineDrop("staff", 2);
	entity.getComponent<Drop>().defineDrop("bow", 2);

	entity.addGroup(Game::groupEnemies);
	entity.addGroup(group);

	entity.update();
	return entity;
}

#endif