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

#include "peakengine/import/XMLWorld.hpp"
#include "peakengine/core/Engine.hpp"
#include "peakengine/core/Game.hpp"
#include "peakengine/support/tinyxml.h"

#include <iostream>

namespace peak
{
	XMLWorld::XMLWorld(Engine *engine, std::string file)
		: World(engine), file(file)
	{
	}
	XMLWorld::~XMLWorld()
	{
	}

	bool XMLWorld::load()
	{
		// Open XML file
		TiXmlDocument xml(file.c_str());
		if (!xml.LoadFile() || xml.Error())
		{
			std::cout << "Could not load XML file " << file << ": "
				<< xml.ErrorDesc() << std::endl;
			return false;
		}
		TiXmlNode *root = xml.FirstChild("World");
		if (!root)
		{
			std::cerr << "Parser error: <World> not found." << std::endl;
			return false;
		}
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
			// Component information
			// TODO: Load components from XML data
			if (!componentelem->Attribute("name"))
			{
				std::cout << "World: Component name missing." << std::endl;
				componentnode = root->IterateChildren("Component", componentnode);
				continue;
			}
			std::string name = componentelem->Attribute("name");
			// Create component
			WorldComponentFactory *factory = getEngine()->getGame()->getWorldComponentFactory(name);
			if (!factory)
			{
				std::cout << "World: Component factory " << name << " not found." << std::endl;
				return false;
			}
			WorldComponent *component = factory->createComponent(this);
			if (!component)
			{
				std::cout << "World: Could not create component." << std::endl;
				return false;
			}
			addComponent(component);
			componentnode = root->IterateChildren("Component", componentnode);
		}
		// Load entities
		TiXmlNode *entitynode = root->FirstChild("Entity");
		while (entitynode)
		{
			TiXmlElement *entityelem = entitynode->ToElement();
			if (!entityelem)
			{
				entitynode = root->IterateChildren("Entity", entitynode);
				continue;
			}
			// Get entity information
			if (!entityelem->Attribute("name"))
			{
				std::cout << "World: Entity name missing." << std::endl;
				entitynode = root->IterateChildren("Entity", entitynode);
				continue;
			}
			std::string name = entityelem->Attribute("name");
			// Create entity
			EntityFactory *factory = getEngine()->getGame()->getEntityFactory(name);
			if (!factory)
			{
				std::cout << "World: Entity factory not found." << std::endl;
				return false;
			}
			Entity *entity = factory->createEntity(this);
			if (!entity)
			{
				std::cout << "World: Could not create entity." << std::endl;
				return false;
			}
			addEntity(entity);
			entitynode = root->IterateChildren("Entity", entitynode);
		}
		return true;
	}
}
