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

#include "peakengine/core/World.hpp"
#include "peakengine/core/Entity.hpp"

namespace peak
{
	World::World(Engine *engine)
		: limiter(20000), threadrunning(false), time(0)
	{
	}
	World::~World()
	{
		// Stop thread
		stop(true);
		// Delete entities
		for (unsigned int i = 0; i < entities.size(); i++)
		{
			if (entities[i])
			{
				delete entities[i];
			}
		}
	}

	void World::addEntity(Entity *entity)
	{
		entitymutex.lock();
		if (freelist.empty())
		{
			// We don't have any free indices in the entity list
			entities.push_back(entity);
			entity->setID(entities.size());
		}
		else
		{
			// Reuse free index
			entities[freelist.front() - 1] = entity;
			entity->setID(freelist.front());
			freelist.pop();
		}
		entitymutex.unlock();
	}
	void World::removeEntity(Entity *entity)
	{
		entitymutex.lock();
		if (threadrunning)
		{
			// We cannot delete the entity directly as it might still be in use
			removalqueue.push(entity->getID());
		}
		else
		{
			// Directly delete entity
			deleteEntity(entity->getID());
		}
		entitymutex.unlock();
	}

	void World::start()
	{
		if (threadrunning)
			return;
		// Start thread
		threadrunning = true;
		stopthread = false;
		pausethread = false;
		thread.create(new ClassFunctor<World>(this, &World::worldThread));
	}
	void World::pause()
	{
		if (!threadrunning)
			return;
		pausethread = true;
	}
	void World::stop(bool wait)
	{
		if (!threadrunning)
			return;
		stopthread = true;
		if (wait)
			thread.wait();
	}

	void World::worldThread()
	{
		while (!stopthread)
		{
			entitymutex.lock();
			if (!pausethread)
			{
				// Update entities
				// TODO: Locking
				for (unsigned int i = 0; i < entities.size(); i++)
				{
					if (entities[i])
					{
						entitymutex.unlock();
						entities[i]->update();
						entitymutex.lock();
					}
				}
			}
			// Delete unneeded entities
			while (!removalqueue.empty())
			{
				unsigned int id = removalqueue.front();
				deleteEntity(id);
			}
			entitymutex.unlock();
			// Limit world to 50 fps
			limiter.wait();
		}
		threadrunning = false;
	}

	void World::deleteEntity(unsigned int id)
	{
		delete entities[id - 1];
		entities[id - 1] = 0;
		freelist.push(id);
	}
}
