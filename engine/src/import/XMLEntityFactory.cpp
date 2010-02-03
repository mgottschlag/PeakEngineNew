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
#include "peakengine/support/tinyxml.h"

#include <iostream>

namespace peak
{
	XMLEntityFactory::XMLEntityFactory(std::string name, std::string file)
		: EntityFactory(name), file(file)
	{
	}
	XMLEntityFactory::~XMLEntityFactory()
	{
	}

	bool XMLEntityFactory::load()
	{
		// Open XML file
		TiXmlDocument xml(file.c_str());
		if (!xml.LoadFile() || xml.Error())
		{
			std::cout << "Could not load XML file " << file << ": "
				<< xml.ErrorDesc() << std::endl;
			return false;
		}
		TiXmlNode *node = xml.FirstChild("Entity");
		if (!node)
		{
			std::cerr << "Parser error: <Entity> not found." << std::endl;
			return false;
		}
		// Load properties
		// TODO
		// Load components
		// TODO
		return true;
	}

	Entity *XMLEntityFactory::createEntity(World *world, bool local)
	{
		return 0;
	}
}
