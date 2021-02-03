
#ifndef INVENTORY_H
#define INVENTORY_H

#include <SDL.h>
#include <cstring>

#include "Game.h"
#include "ItemComponent.h"
#include "UILabel.h"

class InventoryButton
{
public:
	bool active;
	SDL_Rect btn;
	ButtonTexture buttonTexture;
	SDL_Texture * fnt, * tex;
	SDL_Texture * bTex, * bHov, * bPre;
	std::string buttonText;
	SDL_Rect srcR, destR;
	bool pressed = false;
	int buttonPadding = 2;

	InventoryButton()
	{}

	InventoryButton(int xpos, int ypos, int width, int height, int padding, ButtonTexture tex, std::string text) :
		buttonTexture(tex), buttonText(text), buttonPadding(padding)
	{
		srcR.x = 0;
		srcR.y = 0;
		srcR.h = srcR.w = 32;

		destR.w = width;
		destR.h = height;
		destR.x = xpos;
		destR.y = ypos;

		this->tex = Game::assets->GetTexture(buttonTexture.button);
		this->bTex = Game::assets->GetTexture(buttonTexture.button);
		this->bHov = Game::assets->GetTexture(buttonTexture.button_hovered);
		this->bPre = Game::assets->GetTexture(buttonTexture.button_pressed);

		SetButtonText(text);
	}

	~InventoryButton()
	{
		//SDL_DestroyTexture(fnt);
		//SDL_DestroyTexture(tex);

		//SDL_DestroyTexture(bTex);
		//SDL_DestroyTexture(bHov);
		//SDL_DestroyTexture(bPre);
	}

	void SetButtonText(std::string text)
	{
		SDL_Surface * surf = TTF_RenderText_Blended(Game::assets->GetFont(buttonTexture.button_font), text.c_str(), buttonTexture.button_font_color);
		fnt = SDL_CreateTextureFromSurface(Game::renderer, surf);
		SDL_FreeSurface(surf);
		SDL_QueryTexture(fnt, nullptr, nullptr, &btn.w, &btn.h);
		btn.w += buttonPadding * 2;
		btn.h += buttonPadding * 2;
		btn.x = destR.x + ((destR.w - btn.w) / 2);
		btn.y = destR.y + ((destR.h - btn.h) / 2);
	}

	void update()
	{
		Vector2D mouse = getMousePos();
		if(mouse.x < destR.x + destR.w && mouse.y < destR.y + destR.h &&
			mouse.x >= destR.x && mouse.y >= destR.y)
		{
			if (!pressed)
			{
				tex = bHov;
				if(Game::event.type == SDL_MOUSEBUTTONDOWN)
				{
					switch(Game::event.button.button)
					{
						case SDL_BUTTON_LEFT:
							pressed = true;
							tex = bPre;
							break;
						default:
							break;
					}
				}
			}
			if(Game::event.type == SDL_MOUSEBUTTONUP)
			{
				switch(Game::event.button.button)
				{
					case SDL_BUTTON_LEFT:
						pressed = false;
						tex = bHov;
						break;
					default:
						break;
				}
			}
		}
		else
		{
			tex = bTex;
		}
	}

	void draw()
	{
		TextureManager::Draw(tex, destR);
		TextureManager::Draw(fnt, btn);
	}
};

class ItemFrame
{
public:
	ItemComponent item;
	bool isAlreadyOccupied = false;
	SDL_Texture * frame_tex, * item_tex = NULL;
	SDL_Rect destR;
	bool already_pressed, pressed = false;
	const char * ID = "";

	ItemFrame()
	{
		frame_tex = Game::assets->GetTexture("item_frame");
	}

	ItemFrame(int x, int y, int w, int h)
	{
		destR.x = x;
		destR.y = y;
		destR.w = w;
		destR.h = h;
		frame_tex = Game::assets->GetTexture("item_frame");
	}

	~ItemFrame()
	{
		SDL_DestroyTexture(frame_tex);
	}

	void setItem(const char * itemID)
	{
		printf("SETITEM:: %s\n", itemID);
		if (strcmp(itemID, "") == 0)
		{
			printf("SETITEM:: NULL ITEM\n");
			isAlreadyOccupied = false;
		}
		else
		{
			printf("SETITEM:: NON NULL ITEM\n");
			isAlreadyOccupied = true;
			item_tex = Game::assets->GetTexture(itemID);
			item = Game::items[itemID];
		}
		ID = itemID;
	}

	void removeItem()
	{
		isAlreadyOccupied = false;
		item_tex = NULL;
	}

	void update()
	{
		Vector2D mouse = getMousePos();
		if(!already_pressed && pressed)
		{
			if(Game::event.type == SDL_MOUSEBUTTONDOWN)
			{
				switch(Game::event.button.button)
				{
					case SDL_BUTTON_LEFT:
						already_pressed = false;
						pressed = !pressed;
						frame_tex = Game::assets->GetTexture("item_frame");
						break;
					default:
						break;
				}
			}
		}
		if(mouse.x < destR.x + destR.w && mouse.y < destR.y + destR.h &&
			mouse.x >= destR.x && mouse.y >= destR.y)
		{
			if (!already_pressed)
			{
				if(Game::event.type == SDL_MOUSEBUTTONDOWN)
				{
					switch(Game::event.button.button)
					{
						case SDL_BUTTON_LEFT:
							already_pressed = true;
							frame_tex = Game::assets->GetTexture("item_frame_selected");
							pressed = !pressed;
							break;
						default:
							break;
					}
				}
			}
			if(Game::event.type == SDL_MOUSEBUTTONUP)
			{
				switch(Game::event.button.button)
				{
					case SDL_BUTTON_LEFT:
						already_pressed = false;
						break;
					default:
						break;
				}
			}
		}
	}

	void draw()
	{
		TextureManager::Draw(frame_tex, destR);
		if(isAlreadyOccupied)
		{
			TextureManager::Draw(item_tex, destR);
		}
	}
};

class Inventory : public Component
{
public:
	InventoryButton * btn;
	int x, y, width, height, mCols, mLins, padding;
	ItemFrame ** items = NULL;
	ItemFrame * set = NULL;
	SDL_Texture * inventory;
	SDL_Texture * HUD_bar, * HUD_life;
	SDL_Rect HUD_bar_rect, HUD_life_rect;
	SDL_Rect destR;
	bool visible = false, ready = false;
	int selected_x, selected_y;
	Entity * life_label, * item_name;

	Inventory(int xpos, int ypos, int w, int h, const int cols, const int lins, int padd) :
		x(xpos), y(ypos), width(w), height(h), mCols(cols), mLins(lins), padding(padd)
	{
		set = new ItemFrame[3];
		items = new ItemFrame*[cols];
		for(int i = 0; i < cols; i++)
		{
			items[i] = new ItemFrame[lins];
		}
		for(int i = 0; i < cols; i++)
		{
			for(int j = 0; j < lins; j++)
			{
				items[i][j].destR.x = ((w + padding) * i) + x;
				items[i][j].destR.y = ((h + padding) * j) + y;
				items[i][j].destR.w = w;
				items[i][j].destR.h = h;
			}
		}
		for(int i = 0; i < 3; i++)
		{
			set[i].destR.x = w + padding + (x - 100);
			set[i].destR.y = ((h + padding) * i) + (y - 100);
			set[i].destR.w = w;
			set[i].destR.h = h;
		}
		destR.x = x - padding;
		destR.y = y - padding;
		destR.h = ((h + padding) * mLins) + padding;
		destR.w = ((w + padding) * mCols) + padding;

		inventory = Game::assets->GetTexture("interface");

		HUD_bar = Game::assets->GetTexture("HUD_bar");
		HUD_life = Game::assets->GetTexture("HUD_life");
		
		HUD_life_rect.x = HUD_life_rect.y = HUD_bar_rect.x = HUD_bar_rect.y = 0;
		HUD_life_rect.w = HUD_bar_rect.w = 200;
		HUD_life_rect.h = HUD_bar_rect.h = 50;

		life_label = &Game::assets->CreateLabel(Vector2D(100, 15), "0/0", "gamer", Color(255, 255, 255, 255));
		life_label->getComponent<UILabel>().aligned_center = true;

		item_name = &Game::assets->CreateLabel(Vector2D(x, y - 50), "", "gamer", Color(255, 255, 255, 255));
		item_name->getComponent<UILabel>().visible = false;

		SDL_Color black = {0, 0, 0, 255};
		btn = new InventoryButton[3];

		ButtonTexture btnt("menubackpack", "menubackpack_hovered", "menubackpack_pressed", "gamer", black);
		btn[0] = InventoryButton(xpos + destR.w, ypos, 120, 30, 1, btnt, "Equipar");
		btn[1] = InventoryButton(xpos + destR.w, ypos + 30, 120, 30, 1, btnt, "-");
		btn[2] = InventoryButton(xpos + destR.w, ypos + 60, 120, 30, 1, btnt, "Jogar fora");
	}

	~Inventory()
	{
		SDL_DestroyTexture(inventory);
		SDL_DestroyTexture(HUD_bar);
		SDL_DestroyTexture(HUD_life);
	}

	enum setPieces
	{
		weapon,
		armor,
		boots
	};

	int insertItem(const char * itemID, int quantity)
	{
		if(strcmp(itemID, "soul") == 0)
		{
			entity->getComponent<CharacterComponent>().souls += quantity;
			return 1;
		}
		for(int j = 0; j < mLins; j++)
		{
			for(int i = 0; i < mCols; i++)
			{
				if(items[i][j].isAlreadyOccupied == false)
				{
					items[i][j].setItem(itemID);
					return 1;
				}
			}
		}
		return 0;
	}

	void removeItem(int x, int y)
	{
		items[x][y].removeItem();
	}

	void turnVisible()
	{
		item_name->getComponent<UILabel>().visible = visible = !visible;
	}

	void update() override
	{
		std::stringstream life_text;
		life_text << entity->getComponent<CharacterComponent>().life.value << "/" << entity->getComponent<CharacterComponent>().life.max;
		life_label->getComponent<UILabel>().SetLabelText(life_text.str());
		HUD_life_rect.w = static_cast<int>(HUD_bar_rect.w * (entity->getComponent<CharacterComponent>().life.value / entity->getComponent<CharacterComponent>().life.max));
		if (visible)
		{
			btn[0].update();
			btn[1].update();
			btn[2].update();
			set[0].update();
			set[1].update();
			set[2].update();
			for(int i = 0; i < mCols; i++)
			{
				for(int j = 0; j < mLins; j++)
				{
					items[i][j].update();
					if(items[i][j].pressed && !ready)
					{
						ready = true;
						selected_x = i;
						selected_y = j;
						item_name->getComponent<UILabel>().SetLabelText(items[selected_x][selected_y].item.ItemName);
						printf("SELECTED: %i %i\n", i, j);
					}
					
					if (ready)
					{
						if(btn[0].pressed)
						{
							const char * tmp = items[selected_x][selected_y].ID;
							if(tmp == NULL || strcmp(tmp, "") == 0)
							{
								ready = false;
								printf("ITEM VAZIO\n");
								//continue;
							}
							else
							{
								printf("ITEM NÃO VAZIO\n");
								items[selected_x][selected_y].setItem(set[0].ID);
								set[weapon].setItem(tmp);
								ready = false;
							}
						}
						else if(btn[1].pressed)
						{
							printf("SLA:: %i, %i\n", selected_x, selected_y);
							ready = false;
						}
						else if(btn[2].pressed)
						{
							removeItem(selected_x, selected_y);
							ready = false;
						}
						if(!items[selected_x][selected_y].pressed)
						{
							printf("NOT READY\n");
							item_name->getComponent<UILabel>().SetLabelText("");
							ready = false;
						}
					}
				}
			}
		}
	}

	void draw() override
	{
		TextureManager::Draw(HUD_bar, HUD_bar_rect);
		TextureManager::Draw(HUD_life, HUD_life_rect);
		if (visible)
		{
			TextureManager::Draw(inventory, destR);
			for(int i = 0; i < mCols; i++)
			{
				for(int j = 0; j < mLins; j++)
				{
					items[i][j].draw();
				}
			}
			btn[0].draw();
			btn[1].draw();
			btn[2].draw();
			set[0].draw();
			set[1].draw();
			set[2].draw();
		}
	}
};

#endif