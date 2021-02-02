
#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include <map>
#include <memory>

#include "Map.h"

class MapManager
{
private:
	std::map<std::string, std::unique_ptr<Map>> maps;
	std::string currentMap;

public:
	Map& addMap(std::string name, int ms, int ts, const char * file, int twide)
	{
		Map* e = new Map(name, ms, ts);
		e->LoadMap(file, twide);
		std::unique_ptr<Map> uPtr{ e };
		maps.emplace(name, std::move(uPtr));
		return *e;
	}

	void loadMap(std::string name)
	{
		Game::camera.w = maps[name]->getWidth() - WINDOW_WIDTH;
		Game::camera.h = maps[name]->getHeight() - WINDOW_HEIGHT;

		currentMap = name;
	}

	Map& getMap(const std::string& name)
	{
		return *maps[name];
	}

	void update()
	{
		maps[currentMap]->update();
	}

	void draw()
	{
		maps[currentMap]->draw();
	}

};


#endif