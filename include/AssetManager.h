
#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <map>

#include "Lib.h"
#include "ButtonTexture.h"

class AssetManager
{
public:
	AssetManager(Manager * man);
	~AssetManager();

	Entity& CreateProjectile(const Vector2D& position, const Vector2D& target, int range, int speed, int width, int height, int scale, float Rot, const std::string& id, int subGroup, bool points);
	Entity& CreateNPC(const Vector2D& position, int width, int height, int scale, const std::string& id, int aiType, int group);
	Entity& CreateButton(const Vector2D& position, int width, int height, int padding, int sc, const ButtonTexture& tex, const std::string& text, bool act, int subGroup, Function function);
	Entity& CreateLabel(const Vector2D& position, const std::string& text, const std::string& font, SDL_Color color);

	void AddTexture(const std::string& id, char const * path);
	void AddFont(const std::string& id, const std::string& path, int fontSize);

	SDL_Texture* GetTexture(const std::string& id);
	TTF_Font* GetFont(const std::string& id);

private:
	Manager * manager;
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;
	
};

#endif