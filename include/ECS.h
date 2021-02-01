
#ifndef ECS_H
#define ECS_H

#include <iostream>
#include <bitset>
#include <array>
#include <memory>
#include <vector>
#include <algorithm>
#include <map>

class Component;
class Entity;
class Manager;

typedef void (*Function)();

inline size_t getNewComponentTypeID()
{
	static size_t lastID = 0u;
	return lastID++;
}

template <typename T>
inline size_t getNewComponentTypeID() noexcept
{
	static_assert (std::is_base_of<Component, T>::value, "");
	static size_t typeID = getNewComponentTypeID();
	return typeID;
}

constexpr size_t maxComponents = 32;
constexpr size_t maxGroups = 32;

using Group = size_t;

class Component
{
public:
	Entity* entity;

	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}

	virtual ~Component() {}

};

class Entity
{
protected:
	Manager& manager;
	bool active = true;
	std::vector<std::unique_ptr<Component>> components;

	std::array<Component*, maxComponents> componentArray;
	std::bitset<maxComponents> componentBitSet;
	std::bitset<maxGroups> groupBitSet;

public:
	Entity(Manager& mManager):
	manager(mManager)
	{}

	void update()
	{
		for (auto& c: components) c->update();
	}
	void draw()
	{
		for (auto& c: components) c->draw();
	}
	bool isActive() const { return active; }

	bool hasGroup(size_t mGroup)
	{
		return groupBitSet[mGroup];
	}

	void addGroup(size_t mGroup);
	void delGroup(size_t mGroup)
	{
		groupBitSet[mGroup] = false;
	}

	void destroy() { active = false; }

	template <typename T>
	bool hasComponent() const
	{
		return componentBitSet[getNewComponentTypeID<T>()];
	}

	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		componentArray[getNewComponentTypeID<T>()] = c;
		componentBitSet[getNewComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}
	template <typename T>
	T& getComponent() const
	{
		auto ptr(componentArray[getNewComponentTypeID<T>()]);
		return * static_cast<T*>(ptr);
	}
};

class Manager
{
private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity*>, maxGroups> groupedEntities;
	std::map<const char *, Entity*> entityID;
public:
	void update()
	{
		for (auto& e: entities) e->update();
	}

	void draw()
	{
		for (auto& e: entities) e->draw();
	}

	void refresh()
	{
		for(auto i(0u); i < maxGroups; i++)
		{
			auto& v(groupedEntities[i]);
			v.erase(
				std::remove_if(std::begin(v), std::end(v),
					[i](Entity* mEntity)
					{
						return !mEntity->isActive() || !mEntity->hasGroup(i);
					}),
				std::end(v));
		}

		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
			[](const std::unique_ptr<Entity> &mEntity)
		{
			return !mEntity->isActive();
		}),
		std::end(entities));
	}

	void AddToGroup(Entity* mEntity, size_t mGroup)
	{
		groupedEntities[mGroup].emplace_back(mEntity);
	}

	std::vector<Entity*>& getGroup(size_t mGroup)
	{
		return groupedEntities[mGroup];
	}

	Entity& addEntity()
	{
		Entity* e = new Entity(*this);
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}

	Entity& addEntity(const char * id)
	{
		Entity* e = new Entity(*this);
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));

		entityID.emplace(id, e);

		return *e;
	}


	Entity* getEntityByID(const char * id)
	{
		return entityID[id];
	}
};

#endif