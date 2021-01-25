
#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL.h>

#include "Game.h"
#include "TextureManager.h"
#include "TransformComponent.h"
#include "Animation.h"

class SpriteComponent : public Component
{
private:
	TransformComponent * transform;
	SDL_Texture * texture;
	SDL_Rect scrRect, destRect;

public:
	bool animated = false;
	int animIndex = 0;
	int frames = 0;
	int speed = 100;
	int current_frame = 0;
	bool destroyAfter = false, visible = true;
	std::map<const char *, Animation> animations;
	float Rspeed = 0.0f;
	float angle = 0.0f;
	
	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(std::string id)
	{
		setTex(id);
	}

	SpriteComponent(std::string id, bool isAnimated, float Rotation, float ang)
	{
		animated = isAnimated;
		Rspeed = Rotation;
		angle = ang;

		Animation idle = Animation(0, 3, 100);
		Animation walk = Animation(1, 8, 100);
		Animation puff = Animation(2, 8, 100);

		animations.emplace("Idle", idle);
		animations.emplace("Walk", walk);
		animations.emplace("puff", puff);

		Play("Idle");

		setTex(id);
	}

	~SpriteComponent()
	{
		//SDL_DestroyTexture(texture);
	}

	void setTex(std::string id)
	{
		texture = Game::assets->GetTexture(id);
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		scrRect.x = scrRect.y = 0;
		scrRect.w = transform->width;
		scrRect.h = transform->height;
	}

	void update() override
	{
		if (animated)
		{
			current_frame = static_cast<int>((SDL_GetTicks() / speed) % frames);
			if(destroyAfter && current_frame == frames - 1)
				entity->destroy();
			scrRect.x = scrRect.w * current_frame;
		}

		scrRect.y = animIndex * transform->height;

		destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
		destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;

		angle += Rspeed;
		
	}

	void draw() override
	{
		if(visible)
		{
			TextureManager::Draw(texture, scrRect, destRect, angle, spriteFlip);
		}
	}

	void Play(const char* animName)
	{
		animIndex = animations[animName].index;
		frames = animations[animName].frames;
		speed = animations[animName].speed;
		current_frame = 0;
	}

	void Play(int index, int frame_c, int sp)
	{
		animated = true;
		animIndex = index;
		frames = frame_c;
		speed = sp;
		current_frame = 0;
	}
};

#endif