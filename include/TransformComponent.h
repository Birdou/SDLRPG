
#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "ECS.h"
#include "Vector2D.h"

class TransformComponent : public Component
{
public:

	Vector2D last_position, position;
	Vector2D velocity;
	Vector2D relative_center, center;

	int height = 32;
	int width = 32;
	int scale = 1;

	int speed = 3;

	TransformComponent()
	{
		position.Null();
		velocity.Zero();
		center.Null();
		relative_center.Zero();
	}
	
	TransformComponent(int sc)
	{
		position.x = 400;
		position.y = 320;
		scale = sc;
		relative_center.x = (width * scale) / 2;
		relative_center.y = (height * scale) / 2;
		center.x = position.x + relative_center.x;
		center.y = position.y + relative_center.y;
	}

	TransformComponent(float x, float y)
	{
		position.x = x;
		position.y = y;
		relative_center.x = (width * scale) / 2;
		relative_center.y = (height * scale) / 2;
		center.x = position.x + relative_center.x;
		center.y = position.y + relative_center.y;
	}

	TransformComponent(float x, float y, int h, int w, int sc)
	{
		last_position.x = position.x = x;
		last_position.y = position.y = y;
		height = h;
		width = w;
		scale = sc;
		relative_center.x = (width * scale) / 2;
		relative_center.y = (height * scale) / 2;
		center.x = position.x + relative_center.x;
		center.y = position.y + relative_center.y;
		update();
	}

	void init() override
	{
		velocity.Zero();
	}

	void update() override
	{
		last_position = position;
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
		center.x = position.x + relative_center.x;
		center.y = position.y + relative_center.y;
	}
};

#endif