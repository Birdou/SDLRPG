
#ifndef EXAMPLEMENU_H
#define EXAMPLEMENU_H

#include <vector>
#include <map>

#include "Button.h"
#include "Background.h"
#include "UILabel.h"

class MenuManager;

class MenuUI
{
private:
	MenuManager& manager;

public:
	bool visible = false;

	std::map<const char *, Button *> buttons;
	std::map<const char *, Background *> backgrounds;
	std::map<const char *, UILabel *> labels;

	MenuUI(MenuManager& mManager):
	manager(mManager)
	{}

	~MenuUI()
	{
		for(auto& i : backgrounds)
			delete(i.second);
		for(auto& b: buttons)
			delete(b.second);
		for(auto& l: labels)
			delete(l.second);
	}

	void addBackground(const char * id, int xpos, int ypos, int width, int height, std::string tex)
	{
		backgrounds.emplace(id, new Background(xpos, ypos, width, height, tex));
	}

	void addButton(const char * id, int xpos, int ypos, int width, int height, int padding, int sc, ButtonTexture tex, std::string text, bool vis, Function func)
	{
		buttons.emplace(id, new Button(xpos, ypos, width, height, padding, sc, tex, text, vis, func));
	}

	void addLabel(const char * id, int xpos, int ypos, std::string text, std::string font, SDL_Color colour)
	{
		labels.emplace(id, new UILabel(xpos, ypos, text, font, colour));
	}

	void turnVisible()
	{
		visible = !visible;
		for(auto& i : backgrounds)
			i.second->visible = visible;
		for(auto& b : buttons)
			b.second->visible = visible;
		for(auto& l: labels)
			l.second->visible = visible;

	}

	void update()
	{
		if(visible)
		{
			for(auto& b: buttons)
				b.second->update();
		}
	}

	void draw()
	{
		if(visible)
		{
			for(auto& i : backgrounds)
				i.second->draw();
			for(auto& b: buttons)
				b.second->draw();
			for(auto& l: labels)
				l.second->draw();
		}
	}
};

class MenuManager
{
private:
	std::map<std::string, std::unique_ptr<MenuUI>> menus;

public:
	MenuUI& addMenu(std::string name)
	{
		MenuUI* e = new MenuUI(*this);
		std::unique_ptr<MenuUI> uPtr{ e };
		menus.emplace(name, std::move(uPtr));
		return *e;
	}

	MenuUI& getMenu(std::string name)
	{
		return *menus[name];
	}

	void update()
	{
		for(auto& m : menus)
		{
			if(m.second->visible)
			{
				m.second->update();
				return;
			}
		}
	}

	void draw()
	{
		for(auto& m : menus) m.second->draw();
	}

	void turnVisible(std::string menu)
	{
		menus[menu]->turnVisible();
	}

	void disableAll()
	{
		for(auto& m : menus) m.second->visible = false;
	}

};

#endif