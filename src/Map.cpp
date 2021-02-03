
#include "Map.h"

#include "Game.h"
#include "ColliderComponent.h"
#include "Tile.h"

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

			addTile(srcX, srcY, x * scalledSize, y * scalledSize);
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
				Collider* c = new Collider(initial * scalledSize, y * scalledSize, count * scalledSize, scalledSize);
				std::unique_ptr<Collider> uPtr{ c };
				colliders.emplace_back(std::move(uPtr));
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
				Collider* c = new Collider(x * scalledSize, initial * scalledSize, scalledSize, count * scalledSize);
				std::unique_ptr<Collider> uPtr{ c };
				colliders.emplace_back(std::move(uPtr));

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
				Collider* c = new Collider(x * scalledSize, y * scalledSize, scalledSize, scalledSize);
				std::unique_ptr<Collider> uPtr{ c };
				colliders.emplace_back(std::move(uPtr));
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

Tile& Map::addTile(int srcX, int srcY, int xpos, int ypos)
{
	Tile* e = new Tile(srcX, srcY, xpos, ypos, tileSize, mapScale, texID);
	std::unique_ptr<Tile> uPtr{ e };
	tiles.emplace_back(std::move(uPtr));
	return *e;
}

Spawn& Map::addSpawn(int xpos, int ypos, int rng, int mMembers, int freq, EnemyPrototype func, int group)
{
	Spawn* e = new Spawn(xpos, ypos, rng, mMembers, freq, func, group);
	std::unique_ptr<Spawn> uPtr{ e };
	spawns.emplace_back(std::move(uPtr));
	return *e;
}

int Map::getWidth()
{
	return width;
}

int Map::getHeight()
{
	return height;
}
