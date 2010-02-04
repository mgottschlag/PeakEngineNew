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

#include "peakengine/import/XMLGame.hpp"
#include "peakengine/import/ScriptEntityComponentFactory.hpp"
#include "peakengine/import/XMLEntityFactory.hpp"
#include "peakengine/import/XMLWorld.hpp"
#include "peakengine/core/Engine.hpp"
#include "peakengine/support/tinyxml.h"

#include <iostream>

namespace peak
{
	XMLGame::XMLGame()
	{
	}
	XMLGame::~XMLGame()
	{
	}

	bool XMLGame::load()
	{
		std::string filename = getEngine()->getDirectory() + "/Game.xml";
		// Open XML file
		TiXmlDocument xml(filename.c_str());
		if (!xml.LoadFile() || xml.Error())
		{
			std::cout << "Could not load XML file " << filename << ": "
				<< xml.ErrorDesc() << std::endl;
			return false;
		}
		TiXmlNode *root = xml.FirstChild("Game");
		if (!root)
		{
			std::cerr << "Parser error: <Game> not found." << std::endl;
			return false;
		}
		// Load component factories
		TiXmlNode *componentnode = root->FirstChild("Component");
		while (componentnode)
		{
			TiXmlElement *componentelem = componentnode->ToElement();
			if (!componentelem)
			{
				componentnode = root->IterateChildren("Component", componentnode);
				continue;
			}
			if (!componentelem->Attribute("type")
				|| !componentelem->Attribute("name"))
			{
				std::cout << "Game.xml: Component type or name missing." << std::endl;
				componentnode = root->IterateChildren("Component", componentnode);
				continue;
			}
			// Get component factory information
			std::string type = componentelem->Attribute("type");
			std::string name = componentelem->Attribute("name");
			if (getEntityComponentFactory(name))
			{
				std::cout << "Component name already used: " << name << "." << std::endl;
				componentnode = root->IterateChildren("Component", componentnode);
				continue;
			}
			// Create component factory
			if (type == "script")
			{
				ScriptEntityComponentFactory *factory;
				factory = new ScriptEntityComponentFactory(name);
				if (!factory->load(getEngine()->getDirectory() + "/Data/Components/" + name + ".lua"))
				{
					delete factory;
					std::cout << "Could not load script component factory." << std::endl;
					componentnode = root->IterateChildren("Component", componentnode);
					continue;
				}
				addEntityComponentFactory(factory);
			}
			else
			{
				std::cout << "Game.xml: Invalid component type." << std::endl;
			}
			componentnode = root->IterateChildren("Component", componentnode);
		}
		// Load entity factories
		TiXmlNode *entitynode = root->FirstChild("Entity");
		while (entitynode)
		{
			TiXmlElement *entityelem = entitynode->ToElement();
			if (!entityelem)
			{
				entitynode = root->IterateChildren("Entity", entitynode);
				continue;
			}
			// Get entity factory information
			if (!entityelem->Attribute("name"))
			{
				std::cout << "Game.xml: Entity name missing." << std::endl;
				entitynode = root->IterateChildren("Entity", entitynode);
				continue;
			}
			std::string name = entityelem->Attribute("name");
			if (getEntityFactory(name))
			{
				std::cout << "Entity name already used: " << name << "." << std::endl;
				componentnode = root->IterateChildren("Component", componentnode);
				continue;
			}
			// Create entity factory
			std::string file = getEngine()->getDirectory() + "/Data/Entities/" + name + ".xml";
			XMLEntityFactory *factory = new XMLEntityFactory(name, file);
			if (!factory->load())
			{
				delete factory;
				std::cout << "Could not load entity factory." << std::endl;
				entitynode = root->IterateChildren("Entity", entitynode);
				continue;
			}
			addEntityFactory(factory);
			entitynode = root->IterateChildren("Entity", entitynode);
		}
		// Get initial world
		if (root->ToElement()->Attribute("initialworld"))
		{
			initialworld = root->ToElement()->Attribute("initialworld");
		}
		std::cout << "Engine: "<< getEngine() << std::endl;
		return true;
	}
	bool XMLGame::shutdown()
	{
		return false;
	}

	World *XMLGame::createInitialWorld()
	{
		if (initialworld == "")
			return 0;
		// Create XML world
		std::cout << "Engine2: "<< getEngine() << std::endl;
		XMLWorld *world = new XMLWorld(getEngine(),
			getEngine()->getDirectory() + "/Data/Worlds/" + initialworld + ".xml");
		if (!world->load())
		{
			delete world;
			return 0;
		}
		return world;
	}
}
