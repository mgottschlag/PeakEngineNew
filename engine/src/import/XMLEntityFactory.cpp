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

#include "peakengine/import/XMLEntityFactory.hpp"
#include "peakengine/core/World.hpp"
#include "peakengine/core/Game.hpp"
#include "peakengine/core/Engine.hpp"
#include "peakengine/core/Entity.hpp"

#include <iostream>

namespace peak
{
	XMLEntityFactory::XMLEntityFactory(std::string name, std::string file)
		: EntityFactory(name), file(file)
	{
	}
	XMLEntityFactory::~XMLEntityFactory()
	{
		// Delete template
		for (unsigned int i = 0; i < components.size(); i++)
		{
			if (components[i].tpl)
				delete components[i].tpl;
		}
	}

	bool XMLEntityFactory::load()
	{
		// FIXME: Thread safety
		// Open XML file
		if (!xml.LoadFile(file.c_str()) || xml.Error())
		{
			std::cout << "Could not load XML file " << file << ": "
				<< xml.ErrorDesc() << std::endl;
			return false;
		}
		TiXmlNode *root = xml.FirstChild("Entity");
		if (!root)
		{
			std::cerr << "Parser error: <Entity> not found." << std::endl;
			return false;
		}
		// Load properties
		// TODO
		// Load components
		TiXmlNode *componentnode = root->FirstChild("Component");
		while (componentnode)
		{
			TiXmlElement *componentelem = componentnode->ToElement();
			if (!componentelem)
			{
				componentnode = root->IterateChildren("Component", componentnode);
				continue;
			}
			if (!componentelem->Attribute("type"))
			{
				std::cout << "Entity: Component type missing." << std::endl;
				componentnode = root->IterateChildren("Component", componentnode);
				continue;
			}
			ComponentInfo info;
			info.type = componentelem->Attribute("type");
			info.loaded = false;
			info.xml = componentelem;
			info.tpl = 0;
			info.flagsset = 0;
			info.flagsunset = 0;
			if (componentelem->Attribute("flags"))
			{
				// Parse flag conditions
				std::string flags = componentelem->Attribute("flags");
				size_t tokenpos = 0;
				std::string flagstr;
				while (tokenpos < flags.size())
				{
					// Get next flag token
					size_t newtokenpos = flags.find(';', tokenpos);
					if (newtokenpos == std::string::npos)
						newtokenpos = flags.size();
					flagstr = flags.substr(tokenpos, newtokenpos - tokenpos);
					tokenpos = newtokenpos + 1;
					if (flagstr == "")
						continue;
					// Parse flag token
					unsigned int equalpos = flags.find('=');
					std::string flagname = flagstr.substr(0, equalpos);
					bool flagset = flagstr.substr(equalpos + 1,
						flagstr.size() - equalpos - 1) == "yes";
					std::cout << "Flag: " << flagname << ": " << flagset << std::endl;
					EntityFlag flag = Entity::getFlag(flagname);
					if (flag == EEF_Invalid)
					{
						std::cout << "Invalid flag." << std::endl;
						continue;
					}
					// TODO: Warn here
					if (flagset)
						info.flagsset |= flag;
					else
						info.flagsunset |= flag;
				}
			}
			components.push_back(info);
			componentnode = root->IterateChildren("Component", componentnode);
		}
		return true;
	}

	Entity *XMLEntityFactory::createEntity(World *world, unsigned int flags)
	{
		Game *game = world->getEngine()->getGame();
		// Get component factories
		std::vector<EntityComponentFactory*> componentfactories;
		for (unsigned int i = 0; i < components.size(); i++)
		{
			// Check flags
			ComponentInfo &info = components[i];
			std::cout << "Flags: " << flags << "(" << info.flagsunset << "/" << info.flagsset << ")" << std::endl;
			if (((flags & info.flagsunset) != 0)
				|| ((flags & info.flagsset) != info.flagsset))
			{
				componentfactories.push_back(0);
				continue;
			}
			// Retrieve component factory
			EntityComponentFactory *factory = game->getEntityComponentFactory(info.type);
			if (!factory)
			{
				std::cout << "Component factory " << info.type << " not found." << std::endl;
				return 0;
			}
			if (!info.loaded)
			{
				// Load template
				info.tpl = factory->createTemplate(info.xml);
				info.loaded = true;
			}
			componentfactories.push_back(factory);
		}
		// Create entity
		Entity *entity = new Entity(world);
		entity->setType(getName());
		// Create properties
		// TODO
		// Create components
		for (unsigned int i = 0; i < componentfactories.size(); i++)
		{
			if (!componentfactories[i])
				continue;
			EntityComponent *component = componentfactories[i]->createComponent(entity,
				components[i].tpl);
			if (!component)
			{
				std::cout << "Could not create entity component " << components[i].type << std::endl;
				delete entity;
				return 0;
			}
			entity->addComponent(component);
		}
		// Initialize entity
		if (!entity->init())
		{
			std::cout << "Could not initialize entity." << std::endl;
			delete entity;
			return 0;
		}
		return entity;
	}
}
