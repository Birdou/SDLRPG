
#ifndef MENU_H
#define MENU_H

#include <map>

#include "Button.h"
#include "Background.h"
#include "UILabel.h"

class Menu : public Component
{
public:
	bool visible = false;

	std::map<const char *, Button *> buttons;
	std::map<const char *, Background *> backgrounds;
	std::map<const char *, UILabel *> labels;

	Menu()
	{}

	~Menu()
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

	void update() override
	{
		if(visible)
		{
			for(auto& b: buttons)
				b.second->update();
		}
	}

	void draw() override
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

#endif