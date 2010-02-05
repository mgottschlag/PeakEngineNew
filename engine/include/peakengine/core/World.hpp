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

#ifndef _PEAKENGINE_CORE_WORLD_HPP_
#define _PEAKENGINE_CORE_WORLD_HPP_

#include "../support/Thread.hpp"
#include "../support/FrameLimiter.hpp"
#include "../support/Mutex.hpp"

#include <vector>
#include <queue>

namespace peak
{
	class Engine;
	class Entity;
	class WorldComponent;

	/**
	 * A world forms the central class for all scenes in the game, be it game
	 * levels or simply menus. I simply holds a list of entities and updates
	 * at a 20 ms interval within its own thread.
	 *
	 * A world can hold several world components that extend the world, like
	 * a networking component that implements client-server networking or a
	 * a physics component containing static geometry.
	 *
	 * @note Threadsafe.
	 */
	class World
	{
		public:
			/**
			 * Constructor.
			 */
			World(Engine *engine);
			/**
			 * Destructor.
			 */
			virtual ~World();

			/**
			 * Adds an entity to the world and sets its ID accordingly. The
			 * memory management for the entity is then done by the world.
			 */
			void addEntity(Entity *entity);
			/**
			 * Removes an entity from the world and deletes the class instance.
			 */
			void removeEntity(Entity *entity);

			/**
			 * Returns the time of the world. The unit used are 20ms steps.
			 */
			unsigned int getTime()
			{
				return time;
			}

			/**
			 * Adds a world component to the world. The memory management for
			 * the component is then done by the world.
			 */
			void addComponent(WorldComponent *component);
			/**
			 * Returns the world component with the given type.
			 */
			WorldComponent *getComponent(unsigned int type);

			/**
			 * Starts the update loop.
			 */
			void start();
			/**
			 * Pauses the update loop.
			 */
			void pause();
			/**
			 * Stops the update loop completely.
			 */
			void stop(bool wait = false);
			/**
			 * Returns true if the world is currently running.
			 */
			bool isRunning();

			/**
			 * Returns the engine this world was created for.
			 */
			Engine *getEngine()
			{
				return engine;
			}

		private:
			/**
			 * Executes the main loop. Is started from start().
			 */
			void worldThread();
			/**
			 * Really deletes the entity with the given ID. Called either
			 * directly from removeEntity() or from worldThread(). Does not do
			 * any locking.
			 */
			void deleteEntity(unsigned int id);

			Engine *engine;

			Mutex componentmutex;
			std::vector<WorldComponent*> components;

			Mutex entitymutex;
			std::vector<Entity*> entities;
			std::queue<unsigned int> freelist;
			std::queue<unsigned int> removalqueue;

			Mutex threadmutex;
			Thread thread;
			FrameLimiter limiter;
			volatile bool stopthread;
			volatile bool pausethread;
			volatile bool threadrunning;

			unsigned int time;
	};
}

#endif

