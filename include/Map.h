
#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>
#include <memory>

#include "Tile.h"
#include "Spawn.h"
#include "Collider.h"

class Map
{
private:
	std::string texID;
	std::vector<std::unique_ptr<Tile>> tiles;
	std::vector<std::unique_ptr<Spawn>> spawns;
	std::vector<std::unique_ptr<Collider>> colliders;

	int mapScale;
	int tileSize;
	int scalledSize;

	int height;
	int width;

public:
	Map(std::string tID, int ms, int ts);
	~Map();

	void LoadMap(const char * path, int tileswide);
	
	Tile& addTile(int srcX, int srcY, int xpos, int ypos);
	Spawn& addSpawn(int xpos, int ypos, int rng, int mMembers, int freq, EnemyPrototype func, int group);

	int getWidth();
	int getHeight();

	const std::vector<std::unique_ptr<Collider>>& getColliders()
	{
		return colliders;
	}

	void update()
	{
		for(auto& t : tiles) t->update();
		for(auto& s : spawns) s->update();
		for(auto& c : colliders) c->update();
	}

	void draw()
	{
		for(auto& t : tiles) t->draw();
		for(auto& s : spawns) s->draw();
		for(auto& c : colliders) c->draw();
	}

};

#endif