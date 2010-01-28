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

#ifndef _PEAKENGINE_CORE_ENGINE_HPP_
#define _PEAKENGINE_CORE_ENGINE_HPP_

#include "../support/Mutex.hpp"

#include <string>
#include <vector>

namespace peak
{
	class World;
	class Game;

	/**
	 * Main engine class.
	 * @note Threadsafe expect for setGame(), setDirectory()
	 */
	class Engine
	{
		public:
			/**
			 * Constructor.
			 */
			Engine();
			/**
			 * Destructor.
			 */
			~Engine();

			/**
			 * Sets the currently used game definition.
			 */
			void setGame(Game *game);
			/**
			 * Returns the currently used game definition.
			 */
			Game *getGame();
			/**
			 * Sets the working directory for the engine. This can be relative
			 * to the program working directory, but then the latter must not
			 * be changed as long as the engine runs.
			 */
			void setDirectory(std::string directory);
			/**
			 * Returns the engine working directory.
			 */
			std::string getDirectory();

			/**
			 * Adds a world to the engine. Does not start it.
			 */
			void addWorld(World *world);
			/**
			 * Stops a world and removes it from the engine.
			 */
			void removeWorld(World *world);

			/**
			 * Tells all worlds to stop.
			 */
			void stop(bool wait = false);

		private:
			/**
			 * Current engine working directory.
			 */
			std::string directory;
			/**
			 * Currently used game implementation.
			 */
			Game *game;
			/**
			 * List of currently existing worlds.
			 */
			std::vector<World*> worlds;
			/**
			 * Mutex protecting all member functions.
			 */
			Mutex mutex;
	};
}

#endif

