
#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_Mixer.h>
#include <vector>
#include <map>

#include "AssetManager.h"

class ItemComponent;

class Game
{
public:
	Game();
	~Game();

	void init(const char * title, int xpos, int ypos, int width, int height, bool fullscreen);

	void handleEvents();
	void update();
	bool running() { return isRunning; }
	void render();
	void clean();

	static SDL_Renderer * renderer;
	static SDL_Event event;
	static SDL_Rect camera;

	static bool isRunning;
	
	static AssetManager* assets;
	//static Manager manager;

	static std::vector<const char *> itemIDs;
	static std::map<const char *, Entity> NPCs;
	static std::map<const char *, ItemComponent> items;	

	void AddItem(const char * id, const char * path, const char * name, int style, int damage);

	enum gameGroups : std::size_t
	{
		groupMap,
		groupPlayers,
		groupEnemies,
		groupColliders,
		groupEnvironment,

		groupProjectiles,
		groupFriendlyProjectiles,
		groupEnemyProjectiles,
		groupItemProjectiles,

		groupLabels,
		groupInterfaces,//{
			groupUpInterface,
		//}
		groupButtons,//{
			groupPauseButtons,
		//}
		groupSpawns,//{
			groupSpawn1
		//}
	};

	enum itemIDs
	{
		I0,
		sword,
		staff,
		bow,
		IX
	};

	//SDL MIXER THINGS
	enum wavenames
	{
		grass,
		woosh_f1
	};
private:
	SDL_Window * window;
};

void playSound(Game::wavenames waveName, int channel, int times);

Vector2D NearEnemy(Vector2D position, int range);

void turnVisible(Game::gameGroups group);

void pauseMenu();
void backpMenu();
void upMenu();

void closeGame();

void forceDisable(int group);

std::vector<Entity*>& getGroup(Group mGroup);
std::vector<Entity*>& getProximal(Group mGroup);

int Scan(Vector2D point, int range);

Entity& SpawnEnemy(Vector2D position, int group);
Entity& SpawnRat(Vector2D position, int group);

void upCon();
void upStr();
void upInt();
void upDex();

Vector2D GetWindowWH();

Entity& getPlayer();
Manager& getManager();

#endif