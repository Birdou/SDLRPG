
#include "Game.h"

#include "Map.h"
#include "KeyboardController.h"
#include "Menu.h"
#include "AIComponent.h"
#include "Drop.h"

#include "Enemy_rat.h"

Map* map;

Manager manager;
Manager& getManager()
{
	return manager;
}

auto& player(manager.addEntity());
Entity& getPlayer()
{
	return player;
}

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
SDL_Rect Game::camera = {0, 0, 0, 0};

AssetManager* Game::assets = new AssetManager(&manager);

std::vector<const char *> Game::itemIDs;
std::map<const char *, Entity> Game::NPCs;
std::map<const char *, ItemComponent> Game::items;

const int Twavefiles = 2;
Mix_Chunk * mix_chunk[Twavefiles];
const char * wavefiles[Twavefiles] =
{
	"assets/audio/grass_footstep.wav",
	"assets/audio/woosh.mp3"
};

bool Game::isRunning = false;


auto& none(manager.addEntity());

auto& soul_label(manager.addEntity());
auto& pos_label(manager.addEntity());

Game::Game()
{}

Game::~Game()
{}

void Game::init(const char * title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}
		isRunning = true;
		printf("Initialized sucessfuly.\n");
	}

	//SDL_TTF
	if(TTF_Init() == -1)
	{
		printf("Error: SDL_TTF\n");
	}
		memset(mix_chunk, 0, sizeof(Mix_Chunk *) * Twavefiles);
		if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, AUDIO_S16SYS, 2, 512) < 0)
		{
			printf("Error: SDL_Mixer\n");
		}
		if(Mix_AllocateChannels(4) < 0)
		{
			printf("Error: couldn't allocate mix channels\n");
		}
		for(int i = 0; i < Twavefiles; i++)
		{
			mix_chunk[i] = Mix_LoadWAV(wavefiles[i]);
			if(mix_chunk[i] == NULL)
			{
				printf("Error: couldn't load %s\n", wavefiles[i]);
			}
		}

	assets->AddFont("arial", "assets/fonts/arial.ttf", 16);
	assets->AddFont("gamer", "assets/fonts/Gamer.ttf", 28);

		assets->AddTexture("terrain", "assets/terrain_sst.png");
		assets->AddTexture("player", "assets/player_anims.png");
		assets->AddTexture("projectile", "assets/arrow.png");
		assets->AddTexture("friendly_projectile", "assets/proj1.png");
		assets->AddTexture("attack", "assets/attack.png");
		assets->AddTexture("rat", "assets/rat.png");
		assets->AddTexture("melee", "assets/attack.png");
		assets->AddTexture("mage", "assets/magic_sphere.png");
		assets->AddTexture("ranged", "assets/arrow.png");
		assets->AddTexture("ally_life_bar", "assets/ally_life_bar.png");
		assets->AddTexture("enemy_life_bar", "assets/enemy_life_bar.png");
		assets->AddTexture("life_bar_background", "assets/life_bar_background.png");
		assets->AddTexture("HUD_bar", "assets/life_bar.png");
		assets->AddTexture("HUD_life", "assets/life.png");
		assets->AddTexture("button_pause", "assets/button_pause.png");
		assets->AddTexture("backpack", "assets/backpack.png");
		assets->AddTexture("levelup_button", "assets/up.png");
		assets->AddTexture("plus", "assets/upgrade.png");
		assets->AddTexture("background", "assets/background.png");
		assets->AddTexture("button", "assets/button.png");
		assets->AddTexture("button_hovered", "assets/button_hovered.png");
		assets->AddTexture("button_pressed", "assets/button_pressed.png");
		assets->AddTexture("interface", "assets/interface.png");
		assets->AddTexture("item_frame", "assets/item_frame.png");
		assets->AddTexture("item_frame_selected", "assets/item_frame_selected.png");
		assets->AddTexture("menubackpack", "assets/menubackpack.png");
		assets->AddTexture("menubackpack_hovered", "assets/menubackpack_hovered.png");
		assets->AddTexture("menubackpack_pressed", "assets/menubackpack_pressed.png");
		assets->AddTexture("spawneffectframe", "assets/spawneffectframe.png");
		assets->AddTexture("spawnframe", "assets/spawnframe.png");
		assets->AddTexture("soul", "assets/soul.png");
		assets->AddTexture("fit", "assets/fit.png");
		assets->AddTexture("hitbox", "assets/hitbox.png");
		assets->AddTexture("frame", "assets/frame.png");

	AddItem("sword", "assets/sword.png", "Espada do Aprendiz", ItemComponent::sword, 5);
	AddItem("staff", "assets/staff.png", "Cajado do Aprendiz", ItemComponent::staff, 5);
	AddItem("bow", "assets/bow.png", "Arco do Aprendiz", ItemComponent::bow, 5);

	//MAP
	map = new Map("terrain", 4, 32);
	map->LoadMap("assets/maps/map.map", 16);

	//ECS
	camera.w = map->getWidth() - WINDOW_WIDTH;
	camera.h = map->getHeight() - WINDOW_HEIGHT;

	none.addComponent<TransformComponent>();

	player.addComponent<TransformComponent>(800.0f, 640.0f, 32, 32, 4);
	player.addComponent<SpriteComponent>("player", true, 0, 0);
	player.addComponent<KeyboardController>();

	player.addComponent<ColliderComponent>("player");
	player.getComponent<ColliderComponent>().Fit(31, 87, 62, 34);
	player.getComponent<ColliderComponent>().Hit(39, 11, 46, 98);

	player.addGroup(groupPlayers);
	player.addComponent<CharacterComponent>();
	player.addComponent<Inventory>(200, 200, 32, 32, 5, 4, 8);

	soul_label.addComponent<UILabel>(WINDOW_WIDTH - 50, WINDOW_HEIGHT - 50, "n/a", "gamer", Color(255, 255, 255, 255));
	soul_label.getComponent<UILabel>().aligned_right = true;
	soul_label.addGroup(groupLabels);
	//pos_label
	pos_label.addComponent<UILabel>(50, WINDOW_HEIGHT - 50, "n/a", "gamer", Color(255, 255, 255, 255));
	pos_label.addGroup(groupLabels);

	assets->AddSpawn(700, 300, 300, 3, 5, Rat, Game::groupSpawn1);

	//Rat(Vector2D(700, 300), Game::groupEnemies);

	ButtonTexture pausebtn("button_pause", "arial", Color(0, 0, 0, 255));
	ButtonTexture backpbtn("backpack", "arial", Color(0, 0, 0, 255));
	ButtonTexture upbtn("levelup_button", "arial", Color(0, 0, 0, 255));

	assets->CreateButton(WINDOW_WIDTH - 26, 26, 32, 32, 0, 1, pausebtn, "", true, -1, pauseMenu);
	assets->CreateButton(WINDOW_WIDTH - 66, 26, 32, 32, 0, 1, backpbtn, "", true, -1, backpMenu);
	assets->CreateButton(WINDOW_WIDTH - 106, 26, 32, 32, 0, 1, upbtn, "", true, -1, upMenu);

	ButtonTexture stdbtn("button", "button_hovered", "button_pressed", "gamer", Color(0, 0, 0, 255));
	assets->CreateButton(400, 320, 300, 75, 2, 1, stdbtn, "EXIT", false, groupPauseButtons, closeGame);
	assets->CreateButton(400, 220, 300, 75, 2, 1, stdbtn, "-I DON'T DO ANYTHING-", false, groupPauseButtons, nullFunc);

		auto& upmenu(manager.addEntity("upmenu"));
		upmenu.addComponent<Menu>();

		upmenu.getComponent<Menu>().addBackground("background", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, "background");

		upmenu.getComponent<Menu>().addLabel("constitution", 200, 100, "Constituicao", "gamer", Color(255, 255, 255, 255));
		upmenu.getComponent<Menu>().addLabel("strength", 200, 150, "Forca", "gamer", Color(255, 255, 255, 255));
		upmenu.getComponent<Menu>().addLabel("intelligence", 200, 200, "Inteligencia", "gamer", Color(255, 255, 255, 255));
		upmenu.getComponent<Menu>().addLabel("dexterity", 200, 250, "Destreza", "gamer", Color(255, 255, 255, 255));

		upmenu.getComponent<Menu>().addButton("cbtn", 170, 116, 32, 32, 2, 1, ButtonTexture("plus"), "", false, upCon);
		upmenu.getComponent<Menu>().addButton("sbtn", 170, 166, 32, 32, 2, 1, ButtonTexture("plus"), "", false, upStr);
		upmenu.getComponent<Menu>().addButton("ibtn", 170, 216, 32, 32, 2, 1, ButtonTexture("plus"), "", false, upInt);
		upmenu.getComponent<Menu>().addButton("dbtn", 170, 266, 32, 32, 2, 1, ButtonTexture("plus"), "", false, upDex);

		upmenu.addGroup(groupUpInterface);
		upmenu.addGroup(groupInterfaces);
}

//FOR DRAWING
auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles)); 
auto& labels(manager.getGroup(Game::groupLabels));
auto& buttons(manager.getGroup(Game::groupButtons));
auto& interfaces(manager.getGroup(Game::groupInterfaces));
auto& spawns(manager.getGroup(Game::groupSpawns));

//FOR UPDATE
auto& friendlyprojectiles(manager.getGroup(Game::groupFriendlyProjectiles));
auto& enemyprojectiles(manager.getGroup(Game::groupEnemyProjectiles));
auto& itemprojectiles(manager.getGroup(Game::groupItemProjectiles));

void Game::handleEvents()
{
	static int frame;
	if (player.getComponent<TransformComponent>().velocity == Vector2D(0, 0))
		frame = -1;
	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
	if (SDL_PollEvent (&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				isRunning = false;
				break;
			case SDL_KEYDOWN:
				if(player.getComponent<SpriteComponent>().current_frame == 0 ||
					player.getComponent<SpriteComponent>().current_frame == 4)
				{
					if (frame != player.getComponent<SpriteComponent>().current_frame)
					{
						switch(Game::event.key.keysym.sym)
						{
							case SDLK_w:
								Mix_PlayChannel(1, mix_chunk[grass], 0);
								frame = player.getComponent<SpriteComponent>().current_frame;
								break;
							case SDLK_a:
								Mix_PlayChannel(1, mix_chunk[grass], 0);
								frame = player.getComponent<SpriteComponent>().current_frame;
								break;
							case SDLK_s:
								Mix_PlayChannel(1, mix_chunk[grass], 0);
								frame = player.getComponent<SpriteComponent>().current_frame;
								break;
							case SDLK_d:
								Mix_PlayChannel(1, mix_chunk[grass], 0);
								frame = player.getComponent<SpriteComponent>().current_frame;
								break;
							default:
								break;
						}
					}
				}
				break;
			default:
				break;
		}
	}
}

void Game::update()
{

	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;

	std::stringstream souls;
	souls << "souls: " << player.getComponent<CharacterComponent>().souls;
	soul_label.getComponent<UILabel>().SetLabelText(souls.str());

	std::stringstream pp;
	pp << "Player position: " << playerPos;
	pos_label.getComponent<UILabel>().SetLabelText(pp.str());

	if(manager.getEntityByID("upmenu")->getComponent<Menu>().visible)
	{
		std::stringstream css, sss, iss, dss;
		css << "Constituicao nv. " << player.getComponent<CharacterComponent>().constitution << ": " << player.getComponent<CharacterComponent>().conUpCost() << " almas";
		manager.getEntityByID("upmenu")->getComponent<Menu>().labels["constitution"]->SetLabelText(css.str());
		sss << "Forca nv. "<< player.getComponent<CharacterComponent>().strength  << ": " << player.getComponent<CharacterComponent>().strUpCost() << " almas";
		manager.getEntityByID("upmenu")->getComponent<Menu>().labels["strength"]->SetLabelText(sss.str());
		iss << "Inteligencia nv. " << player.getComponent<CharacterComponent>().intelligence << ": " << player.getComponent<CharacterComponent>().intUpCost() << " almas";
		manager.getEntityByID("upmenu")->getComponent<Menu>().labels["intelligence"]->SetLabelText(iss.str());
		dss << "Destreza nv. " << player.getComponent<CharacterComponent>().dexterity << ": " << player.getComponent<CharacterComponent>().dexUpCost() << " almas";
		manager.getEntityByID("upmenu")->getComponent<Menu>().labels["dexterity"]->SetLabelText(dss.str());
	}

	//WORLD COLLIDERS
	SDL_Rect playerFit = player.getComponent<ColliderComponent>().fit;
	for (auto& c : colliders)
	{
		for (auto& p : players)
		{
			SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
			if(Collision::AABB(cCol, playerFit))
			{
				p->getComponent<TransformComponent>().position = p->getComponent<TransformComponent>().last_position;
			}
		}
	}

	//PROJECTILES
	for (auto& a : players)
	{
		for (auto& p : enemyprojectiles)
		{
			if(Collision::AABB(a->getComponent<ColliderComponent>().hitBox, p->getComponent<ColliderComponent>().collider))
			{
				int damage = p->getComponent<ProjectileComponent>().damage;
				if(p->getComponent<SpriteComponent>().destroyAfter)
				{
					if(p->getComponent<SpriteComponent>().frames - 1 == p->getComponent<SpriteComponent>().current_frame)
					{
						a->getComponent<CharacterComponent>().life.value -= damage;
					}
				}
				else
				{
					a->getComponent<CharacterComponent>().life.value -= damage;
					p->getComponent<SpriteComponent>().visible = false;
					p->destroy();
				}
			}
		}
	}
	for (auto& e : enemies)
	{
		for (auto& fp : friendlyprojectiles)
		{
			if(Collision::AABB(e->getComponent<ColliderComponent>().hitBox, fp->getComponent<ColliderComponent>().collider))
			{
				int damage = fp->getComponent<ProjectileComponent>().damage;
				e->getComponent<CharacterComponent>().life.value -= damage;
				fp->getComponent<SpriteComponent>().visible = false;

				std::stringstream stream;
				stream << damage;
				Vector2D pos = e->getComponent<TransformComponent>().center;
				pos.x -= camera.x;
				pos.y -= camera.y;
				auto& label = assets->CreateLabel(pos, stream.str(), "gamer", Color(255, 122, 122, 255));
				label.addComponent<AIComponent>(AIComponent::damageLabel);
				
				if (e->getComponent<CharacterComponent>().life.value <= 0)
				{
					if (e->hasComponent<Drop>())
						e->getComponent<Drop>().spawnDrop();
					auto& soul = Game::assets->CreateProjectile(e->getComponent<TransformComponent>().center, player.getComponent<TransformComponent>().center, 999, 0.5, 32, 32, 1, 0, "soul", Game::groupItemProjectiles, true);
					soul.getComponent<SpriteComponent>().Play(0, 8, 100);
					soul.getComponent<ProjectileComponent>().IID = "soul";
					soul.addComponent<AIComponent>(AIComponent::soul);
					soul.getComponent<ProjectileComponent>().damage = e->getComponent<CharacterComponent>().souls;
					e->getComponent<SpriteComponent>().visible = false;
					e->destroy();
				}
				fp->destroy();
			}
		}
	}
	for (auto& p : itemprojectiles)
	{
		if(Collision::AABB(playerCol, p->getComponent<ColliderComponent>().hitBox))
		{
			if(player.getComponent<Inventory>().insertItem(p->getComponent<ProjectileComponent>().IID, p->getComponent<ProjectileComponent>().damage))
			{
				p->getComponent<SpriteComponent>().visible = false;
				p->destroy();
			}
		}
	}

	manager.refresh();
	manager.update();

	Vector2D playerCenter = player.getComponent<TransformComponent>().center;
	camera.x = playerCenter.x - (WINDOW_WIDTH / 2);
	camera.y = playerCenter.y - (WINDOW_HEIGHT / 2);

	if (playerPos.x < 0) player.getComponent<TransformComponent>().position.x = 0;
	if (playerPos.y < 0) player.getComponent<TransformComponent>().position.y = 0;
	if (playerPos.x > camera.w + WINDOW_WIDTH - playerCol.w) player.getComponent<TransformComponent>().position.x = camera.w + WINDOW_WIDTH - playerCol.w;
	if (playerPos.y > camera.h + WINDOW_HEIGHT - playerCol.h) player.getComponent<TransformComponent>().position.y = camera.h + WINDOW_HEIGHT - playerCol.h;

	if (camera.x < 0) camera.x = 0;
	if (camera.y < 0) camera.y = 0;
	if (camera.x > camera.w) camera.x = camera.w;
	if (camera.y > camera.h) camera.y = camera.h;

}

void Game::render()
{
	//RENDER LAYERS
	SDL_RenderClear(renderer);

	for (auto& t : tiles)
	{
		t->draw();
	}
	for (auto& s : spawns)
	{
		s->draw();
	}
	for(auto& c : colliders)
	{
		c->draw();
	}
	for (auto& e : enemies)
	{
		e->draw();
	}
	for (auto& p : projectiles)
	{
		p->draw();
	}
	for (auto& p : players)
	{
		p->draw();
	}

	for (auto& l : labels)
	{
		l->draw();
	}
	for (auto& i : interfaces)
	{
		i->draw();
	}
	for (auto& b : buttons)
	{
		b->draw();
	}

	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	printf("Mix: Free chunk (*)...\n");
	for(int i = 0; i < Twavefiles; i++)
	{
		Mix_FreeChunk(mix_chunk[i]);
	}
	printf("Mix: Close audio...\n");
	Mix_CloseAudio();
	printf("SDL: Destroy window\n");
	SDL_DestroyWindow(window);
	printf("SDL: Destroy renderer\n");
	SDL_DestroyRenderer(renderer);
	printf("SDL: Quit\n");
	SDL_Quit();
}

void Game::AddItem(const char * id, const char * path, const char * name, int style, int damage)
{
	assets->AddTexture(id, path);
	Game::itemIDs.emplace_back(id);
	Game::items.emplace(id, ItemComponent(id, name, style));
	Game::items[id].damage = damage;
	printf("ADDITEM:: (%s, %i)\n", items[id].ItemName.c_str(), Game::items[id].style);
}

void playSound(Game::wavenames waveName, int channel, int times)
{
	Mix_PlayChannel(channel, mix_chunk[waveName], times);
}

Vector2D NearEnemy(Vector2D position, int range)
{
	int nearest = range;
	float distance;
	for (auto& e : enemies)
	{
		distance = sqrt(pow(e->getComponent<TransformComponent>().center.x - position.x, 2) + pow(e->getComponent<TransformComponent>().center.y - position.y, 2));
		if(distance < nearest)
		{
			return e->getComponent<TransformComponent>().center;
		}
	}
	return Vector2D(-1, -1);
}

void turnVisible(Game::gameGroups group)
{
	auto& enities(manager.getGroup(group));
	for (auto& e : enities)
	{
		if (e->hasComponent<Button>())
			e->getComponent<Button>().visible = e->getComponent<Button>().visible ? false : true;
	}
}

void pauseMenu()
{
	player.getComponent<Inventory>().visible = false;
	manager.getEntityByID("upmenu")->getComponent<Menu>().visible = false;
	turnVisible(Game::groupPauseButtons);
}

void backpMenu()
{
	forceDisable(Game::groupPauseButtons);
	manager.getEntityByID("upmenu")->getComponent<Menu>().visible = false;
	player.getComponent<Inventory>().turnVisible();
}

void upMenu()
{
	player.getComponent<Inventory>().visible = false;
	forceDisable(Game::groupPauseButtons);
	manager.getEntityByID("upmenu")->getComponent<Menu>().turnVisible();
}

void closeGame()
{
	Game::isRunning = false;
}

void forceDisable(int group)
{
	auto& enities(manager.getGroup(group));
	for (auto& e : enities)
	{
		if (e->hasComponent<Button>())
			e->getComponent<Button>().visible = false;
	}
}

std::vector<Entity*>& getGroup(Group mGroup)
{
	auto& entities(manager.getGroup(mGroup));
	return entities;
}

std::vector<Entity*>& getProximal(Group mGroup)
{
	auto& entities(manager.getGroup(mGroup));
	return entities;
}

int Scan(Vector2D point, int range)
{
	int count = 0;
	for (auto& e : enemies)
	{
		if (e->getComponent<TransformComponent>().center.x < point.x + range &&
			e->getComponent<TransformComponent>().center.y < point.x + range &&
			e->getComponent<TransformComponent>().center.x > point.x - range &&
			e->getComponent<TransformComponent>().center.y > point.y - range)
		{
			count++;
		}
	}
	return count;
}

Entity& SpawnEnemy(Vector2D position, int group)
{
	auto& enemy = Game::assets->CreateNPC(position, 32, 32, 4, "enemy", AIComponent::enemy, Game::groupEnemies);
	enemy.addComponent<Drop>();
	enemy.getComponent<Drop>().defineDrop("sword", 5);
	enemy.getComponent<Drop>().defineDrop("staff", 5);
	enemy.getComponent<Drop>().defineDrop("bow", 5);
	enemy.addGroup(group);
	enemy.update();
	return enemy;
}

Entity& SpawnRat(Vector2D position, int group)
{
	auto& enemy = Game::assets->CreateNPC(position, 32, 32, 4, "rat", AIComponent::rat, Game::groupEnemies);
	
	enemy.getComponent<CharacterComponent>().dmg = 2;
	enemy.getComponent<CharacterComponent>().basedmg = 1;
	enemy.getComponent<CharacterComponent>().souls = 16;
	enemy.getComponent<CharacterComponent>().life.max = 50;

	enemy.getComponent<ColliderComponent>().Fit(32, 68, 73, 18);
	enemy.getComponent<ColliderComponent>().Hit(34, 40, 81, 41);

	enemy.addComponent<Drop>();
	enemy.getComponent<Drop>().defineDrop("sword", 15);
	enemy.getComponent<Drop>().defineDrop("staff", 2);
	enemy.getComponent<Drop>().defineDrop("bow", 2);

	enemy.addGroup(group);
	enemy.update();
	return enemy;
}

void upCon()
{
	if(player.getComponent<CharacterComponent>().souls >= player.getComponent<CharacterComponent>().conUpCost())
	{
		player.getComponent<CharacterComponent>().souls -= player.getComponent<CharacterComponent>().conUpCost();
		player.getComponent<CharacterComponent>().constitution++;
		player.getComponent<CharacterComponent>().level++;
	}
}

void upStr()
{
	if(player.getComponent<CharacterComponent>().souls >= player.getComponent<CharacterComponent>().strUpCost())
	{
		player.getComponent<CharacterComponent>().souls -= player.getComponent<CharacterComponent>().strUpCost();
		player.getComponent<CharacterComponent>().strength++;
		player.getComponent<CharacterComponent>().level++;
	}
}

void upInt()
{
	if(player.getComponent<CharacterComponent>().souls >= player.getComponent<CharacterComponent>().intUpCost())
	{
		player.getComponent<CharacterComponent>().souls -= player.getComponent<CharacterComponent>().intUpCost();
		player.getComponent<CharacterComponent>().intelligence++;
		player.getComponent<CharacterComponent>().level++;
	}
}

void upDex()
{
	if(player.getComponent<CharacterComponent>().souls >= player.getComponent<CharacterComponent>().dexUpCost())
	{
		player.getComponent<CharacterComponent>().souls -= player.getComponent<CharacterComponent>().dexUpCost();
		player.getComponent<CharacterComponent>().dexterity++;
		player.getComponent<CharacterComponent>().level++;
	}
}

