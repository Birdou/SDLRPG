
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

	Entity& CreateProjectile(Vector2D pos, Vector2D target, int range, int speed, int width, int height, int scale, float Rot, std::string id, int subGroup, bool points);
	Entity& CreateNPC(Vector2D pos, int width, int height, int scale, std::string id, int aiType, int group);
	Entity& CreateButton(int xpos, int ypos, int width, int height, int padding, int sc, ButtonTexture tex, std::string text, bool act, int subGroup, Function function);
	Entity& CreateLabel(Vector2D pos, std::string text, std::string font, SDL_Color color);

	void AddTexture(std::string id, char const * path);

	SDL_Texture * GetTexture(std::string id);

	void AddFont(std::string id, std::string path, int fontSize);
	TTF_Font * GetFont(std::string id);

private:
	Manager * manager;
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;
	
};

#endif