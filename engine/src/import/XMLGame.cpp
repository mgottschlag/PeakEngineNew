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
		TiXmlNode *node = xml.FirstChild("Game");
		if (!node)
		{
			std::cerr << "Parser error: <Game> not found." << std::endl;
			return false;
		}
		// Load component factories
		// TODO
		// Load entity factories
		// TODO
		// Get initial world
		// TODO
		return true;
	}
	bool XMLGame::shutdown()
	{
		return false;
	}
}
