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

#include "peakengine/core/Engine.hpp"
#include "peakengine/core/Game.hpp"
#include "peakengine/core/World.hpp"

namespace peak
{
	Engine::Engine() : game(0)
	{
	}
	Engine::~Engine()
	{
	}

	void Engine::setGame(Game *game)
	{
		this->game = game;
	}
	Game *Engine::getGame()
	{
		return game;
	}
	void Engine::setDirectory(std::string directory)
	{
		this->directory = directory;
	}
	std::string Engine::getDirectory()
	{
		return directory;
	}

	void Engine::addWorld(World *world)
	{
		mutex.lock();
		worlds.push_back(world);
		mutex.unlock();
	}
	void Engine::removeWorld(World *world)
	{
		mutex.lock();
		for (unsigned int i = 0; i < worlds.size(); i++)
		{
			if (worlds[i] == world)
			{
				worlds.erase(worlds.begin() + i);
				break;
			}
		}
		mutex.unlock();
	}

	void Engine::stop(bool wait)
	{
		for (unsigned int i = 0; i < worlds.size(); i++)
		{
			worlds[i]->stop(wait);
		}
	}
}
