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
			components.push_back(info);
			componentnode = root->IterateChildren("Component", componentnode);
		}
		return true;
	}

	Entity *XMLEntityFactory::createEntity(World *world, bool local)
	{
		Game *game = world->getEngine()->getGame();
		// Get component factories
		std::vector<EntityComponentFactory*> componentfactories;
		for (unsigned int i = 0; i < components.size(); i++)
		{
			ComponentInfo &info = components[i];
			EntityComponentFactory *factory = game->getEntityComponentFactory(info.type);
			if (!factory)
				return 0;
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
			EntityComponent *component = componentfactories[i]->createComponent(entity,
				components[i].tpl);
			if (!component)
			{
				delete entity;
				return 0;
			}
			entity->addComponent(component);
		}
		// Initialize entity
		if (!entity->init())
		{
			delete entity;
			return 0;
		}
		return entity;
	}
}
