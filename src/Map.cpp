
#include "Map.h"

#include "Game.h"
#include "ColliderComponent.h"
#include "TileComponent.h"

Map::Map(std::string tID, int ms, int ts) : texID(tID), mapScale(ms), tileSize(ts)
{
	scalledSize = ms * ts;
}

Map::~Map()
{}

void Map::LoadMap(const char * path, int tileswide)
{
	FILE * mapFile = fopen(path, "r");
	if(mapFile == NULL)
	{
		printf("LOADMAP:: INVALID PATH \"%s\"\n", path);
		return;
	}

	int srcX, srcY, sizeX, sizeY, index;
	fscanf(mapFile, "%i %i\n", &sizeX, &sizeY);
	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			fscanf(mapFile, "%i,", &index);

			srcY = (index / tileswide) * tileSize;
			srcX = (index % tileswide) * tileSize;

			AddTile(srcX, srcY, x * scalledSize, y * scalledSize, Game::groupMap);
		}
	}

	int count = 0, optimized = 0, total = 0;
	int ** map = new int*[sizeY];
	for(int i = 0; i < sizeY; i++)
		map[i] = new int[sizeX];

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			fscanf(mapFile, "%i,", &map[y][x]);
			if(map[y][x] != 0 && map[y][x] != -1)
				printf("LOADMAP:: COLLISION MAP IRREGULARITY AT (%i, %i): %i\n", x, y, map[y][x]);
		}
	}

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			int initial = x;
			if(map[y][x] == -1) continue;
			while (map[y][x] == 0)
			{
				if(count > 0)
				{
					map[y][x - 1] = map[y][x] = -1;
				}
				count++; x++;
			}
			if(count > 1)
			{
				auto& tcol(getManager().addEntity());
				tcol.addComponent<ColliderComponent>("terrain", initial * scalledSize, y * scalledSize, count * scalledSize, scalledSize);
				tcol.addGroup(Game::groupColliders);
				optimized += count;
				total++;
			}
			count = 0;
		}
	}
	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			int initial = y;
			if(map[y][x] == -1) continue;
			while (map[y][x] == 0)
			{
				if(count > 0)
				{
					map[y - 1][x] = map[y][x] = -1;
				}
				count++; y++;
			}
			if(count > 1)
			{
				auto& tcol(getManager().addEntity());
				tcol.addComponent<ColliderComponent>("terrain", x * scalledSize, initial * scalledSize, scalledSize, count * scalledSize);
				tcol.addGroup(Game::groupColliders);
				optimized += count;
				total++;
			}
			count = 0;
		}
	}
	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			if(map[y][x] == 0)
			{
				auto& tcol(getManager().addEntity());
				tcol.addComponent<ColliderComponent>("terrain", x * scalledSize, y * scalledSize, scalledSize, scalledSize);
				tcol.addGroup(Game::groupColliders);
				total++;
			}
		}
	}
	delete[] map;
	printf("LOADMAP:: OPTIMIZED TILES: %i\n", optimized);
	printf("LOADMAP:: TOTAL COLLIDERS: %i\n", total);
	height = sizeY * scalledSize;
	width = sizeX * scalledSize;

	fclose(mapFile);
}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos, int group)
{
	auto& tile(getManager().addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, texID);
	tile.addGroup(group);
}

int Map::getWidth()
{
	return width;
}

int Map::getHeight()
{
	return height;
}
