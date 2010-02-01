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

#ifndef _PEAKENGINE_CORE_GAME_HPP_
#define _PEAKENGINE_CORE_GAME_HPP_

#include "EntityFactory.hpp"
#include "EntityComponentFactory.hpp"

#include <string>
#include <map>

namespace peak
{
	class Engine;

	/**
	 * Main interface providing the basic game actions. This class has to be
	 * implemented, the engine only provides an abstract interface for most
	 * functions.
	 */
	class Game
	{
		public:
			/**
			 * Constructor.
			 */
			Game() : engine(0)
			{
			}
			/**
			 * Destructor.
			 */
			virtual ~Game()
			{
				std::map<std::string, EntityFactory*>::iterator it = factories.begin();
				while (it != factories.end())
				{
					delete it->second;
					it++;
				}
			}
			/**
			 * Initializes the game. Has to be called by the user at startup.
			 */
			virtual bool load() = 0;
			/**
			 * Shuts the engine down. Has to be called when the program is
			 * closed.
			 */
			virtual bool shutdown() = 0;

			/**
			 * Sets the engine instance this game is using.
			 */
			void setEngine(Engine *engine)
			{
				this->engine = engine;
			}
			/**
			 * Returns the engine instance this game is using.
			 */
			Engine *getEngine()
			{
				return engine;
			}

			/**
			 * Adds an entity factory to the game definition. The factory
			 * must be allocated manually as it is deleted when the game is
			 * destroyed.
			 */
			void addEntityFactory(EntityFactory *factory)
			{
				factories.insert(std::pair<std::string, EntityFactory*>(factory->getName(), factory));
			}
			/**
			 * Returns the entity factory with the specified name.
			 */
			EntityFactory *getEntityFactory(std::string name)
			{
				std::map<std::string, EntityFactory*>::iterator it = factories.find(name);
				if (it == factories.end())
					return 0;
				return it->second;
			}

			/**
			 * Adds an entity component factory to the game definition.
			 * The factory must be allocated manually as it is deleted when the
			 * game is destroyed.
			 */
			void addEntityComponentFactory(EntityComponentFactory *factory)
			{
				compfactories.insert(std::pair<std::string, EntityComponentFactory*>(factory->getName(), factory));
			}
			/**
			 * Returns the entity component factory with the specified name.
			 */
			EntityComponentFactory *getEntityComponentFactory(std::string name)
			{
				std::map<std::string, EntityComponentFactory*>::iterator it = compfactories.find(name);
				if (it == compfactories.end())
					return 0;
				return it->second;
			}
		private:
			Engine *engine;
			std::map<std::string, EntityFactory*> factories;
			std::map<std::string, EntityComponentFactory*> compfactories;
	};
}

#endif

