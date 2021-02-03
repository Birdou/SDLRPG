
#include "AssetManager.h"

#include "AIComponent.h"
#include "ItemComponent.h"
#include "Button.h"

AssetManager::AssetManager(Manager * man) : manager(man)
{}

AssetManager::~AssetManager()
{}

Entity& AssetManager::CreateProjectile(const Vector2D& pos, const Vector2D& target, int range, int speed, int width, int height, int scale, float Rot, const std::string& id, int subGroup, bool points)
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

Entity& AssetManager::CreateNPC(const Vector2D& pos, int width, int height, int scale, const std::string& id, int aiType, int group)
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

Entity& AssetManager::CreateButton(const Vector2D& pos, int width, int height, int padding, int sc, const ButtonTexture& tex, const std::string& text, bool vis, int subGroup, Function function)
{
	auto& button(manager->addEntity());
	button.addComponent<Button>(pos.x, pos.y, width, height, padding, sc, tex, text, vis, function);
	button.addGroup(Game::groupButtons);
	if(subGroup >= 0)
		button.addGroup(subGroup);
	return button;
}

Entity& AssetManager::CreateLabel(const Vector2D& pos, const std::string& text, const std::string& font, SDL_Color color)
{
	auto& label(manager->addEntity());
	label.addComponent<UILabel>(pos.x, pos.y, text, font, color);
	label.addGroup(Game::groupLabels);
	return label;
}

void AssetManager::AddTexture(const std::string& id, char const * path)
{
	textures.emplace(id, TextureManager::LoadTexture(path));
}

SDL_Texture * AssetManager::GetTexture(const std::string& id)
{
	return textures[id];
}

void AssetManager::AddFont(const std::string& id, const std::string& path, int fontSize)
{
	fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font * AssetManager::GetFont(const std::string& id)
{
	return fonts[id];
}