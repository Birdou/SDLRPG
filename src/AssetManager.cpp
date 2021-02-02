
#include "AssetManager.h"

#include "AIComponent.h"
#include "ItemComponent.h"
#include "Button.h"

AssetManager::AssetManager(Manager * man) : manager(man)
{}

AssetManager::~AssetManager()
{}

Entity& AssetManager::CreateProjectile(Vector2D pos, Vector2D target, int range, int speed, int width, int height, int scale, float Rot, std::string id, int subGroup, bool points)
{
	auto& projectile(manager->addEntity());

	float ang = 0;
	Vector2D velocity(speed, 0);
	projectile.addComponent<TransformComponent>(pos.x - (width * scale / 2), pos.y - (height * scale / 2), width, height, scale);
	if (points)
	{
		ang = Ang(pos, target);
		velocity.Rotate(ang);
	}
	else
	{
		velocity.Rotate(Ang(pos, target));
	}
	
	projectile.addComponent<ProjectileComponent>(range, speed, velocity);
	projectile.addComponent<ColliderComponent>("projectile");
	projectile.addComponent<SpriteComponent>(id, false, Rot, deg(ang));
	projectile.addGroup(Game::groupProjectiles);
	projectile.addGroup(subGroup);
	projectile.update();
	return projectile;
}

Entity& AssetManager::CreateNPC(Vector2D pos, int width, int height, int scale, std::string id, int aiType, int group)
{
	auto& NPC(manager->addEntity());
	NPC.addComponent<TransformComponent>(pos.x, pos.y, width, height, scale);
	NPC.addComponent<SpriteComponent>(id, true, 0, 0);
	NPC.getComponent<SpriteComponent>().visible = false;
	NPC.addComponent<ColliderComponent>(id);
	NPC.addComponent<AIComponent>(aiType);
	NPC.addComponent<CharacterComponent>();
	NPC.addGroup(group);
	return NPC;
}

Entity& AssetManager::CreateButton(int xpos, int ypos, int width, int height, int padding, int sc, ButtonTexture tex, std::string text, bool vis, int subGroup, Function function)
{
	auto& button(manager->addEntity());
	button.addComponent<Button>(xpos, ypos, width, height, padding, sc, tex, text, vis, function);
	button.addGroup(Game::groupButtons);
	if(subGroup >= 0)
		button.addGroup(subGroup);
	return button;
}

Entity& AssetManager::CreateLabel(Vector2D pos, std::string text, std::string font, SDL_Color color)
{
	auto& label(manager->addEntity());
	label.addComponent<UILabel>(pos.x, pos.y, text, font, color);
	label.addGroup(Game::groupLabels);
	return label;
}

void AssetManager::AddTexture(std::string id, char const * path)
{
	textures.emplace(id, TextureManager::LoadTexture(path));
}

SDL_Texture * AssetManager::GetTexture(std::string id)
{
	return textures[id];
}

void AssetManager::AddFont(std::string id, std::string path, int fontSize)
{
	fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font * AssetManager::GetFont(std::string id)
{
	return fonts[id];
}