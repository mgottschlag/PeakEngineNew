/*
Copyright (c) 2010, Mathias Gottschlag

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "peakengine/core/Entity.hpp"
#include "peakengine/core/Property.hpp"
#include "peakengine/core/EntityComponent.hpp"

namespace peak
{
	Entity::Entity(World *world) : world(world), id(0), flags(0)
	{
	}
	Entity::~Entity()
	{
		// Delete components
		for (unsigned int i = 0; i < components.size(); i++)
		{
			components[i]->destroy();
		}
		for (unsigned int i = 0; i < components.size(); i++)
		{
			delete components[i];
		}
		// Delete properties
		std::map<std::string, Property*>::iterator it = properties.begin();
		while (it != properties.end())
		{
			delete it->second;
			it++;
		}
	}

	void Entity::setID(unsigned int id)
	{
		this->id = id;
	}
	unsigned int Entity::getID()
	{
		return id;
	}

	bool Entity::getFlag(EntityFlag flag)
	{
		return (flags & flag) != 0;
	}
	void Entity::setFlag(EntityFlag flag, bool value)
	{
		if (value)
			flags |= flag;
		else
			flags &= ~flag;
	}
	unsigned int Entity::getFlags()
	{
		return flags;
	}
	void Entity::setFlags(unsigned int flags)
	{
		this->flags = flags;
	}
	EntityFlag Entity::getFlag(const std::string &name)
	{
		if (name == "server")
			return EEF_Server;
		if (name == "client")
			return EEF_Client;
		if (name == "local")
			return EEF_Local;
		return EEF_Invalid;
	}

	void Entity::setType(std::string type)
	{
		this->type = type;
	}

	Property *Entity::addProperty(Property *property, std::string name)
	{
		mutex.lock();
		std::map<std::string, Property*>::iterator it = properties.find(name);
		if (it == properties.end())
		{
			// Add property
			properties.insert(std::pair<std::string, Property*>(name, property));
		}
		else
		{
			// Check and return existing property
			if (property->getType() == it->second->getType())
			{
				delete property;
				property = it->second;
			}
			else
			{
				delete property;
				property = 0;
			}
		}
		mutex.unlock();
		return property;
	}
	Property *Entity::getProperty(std::string name)
	{
		Property *property = 0;
		mutex.lock();
		std::map<std::string, Property*>::iterator it = properties.find(name);
		if (it != properties.end())
			property = it->second;
		mutex.unlock();
		return property;
	}

	void Entity::addComponent(EntityComponent *component)
	{
		components.push_back(component);
	}
	EntityComponent *Entity::getComponent(int type, unsigned int n)
	{
		unsigned int found = 0;
		for (unsigned int i = 0; i < components.size(); i++)
		{
			if (components[i]->getType() == type)
			{
				found++;
				if (found > n)
					return components[i];
			}
		}
		return 0;
	}
	EntityComponent *Entity::getComponent(int type)
	{
		return getComponent(type, 0);
	}

	void Entity::addedToWorld()
	{
		for (unsigned int i = 0; i < components.size(); i++)
		{
			components[i]->addedToWorld();
		}
	}

	bool Entity::init()
	{
		// Install properties
		for (unsigned int i = 0; i < components.size(); i++)
		{
			components[i]->installProperties();
		}
		// Initialize properties
		for (unsigned int i = 0; i < components.size(); i++)
		{
			components[i]->init();
		}
		// TODO: Error handling
		return true;
	}

	void Entity::update()
	{
		for (unsigned int i = 0; i < components.size(); i++)
		{
			components[i]->update();
		}
	}
}
