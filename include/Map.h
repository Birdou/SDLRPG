
#ifndef MAP_H
#define MAP_H

#include <string>

class Map
{
public:
	Map(std::string tID, int ms, int ts);
	~Map();

	void LoadMap(const char * path, int tileswide);
	void AddTile(int srcX, int srcY, int xpos, int ypos, int group);

	int getWidth();
	int getHeight();

private:
	std::string texID;
	int mapScale;
	int tileSize;
	int scalledSize;

	int height;
	int width;

};

#endif