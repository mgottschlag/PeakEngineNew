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

#ifndef _PEAKENGINE_CORE_WORLDCOMPONENTFACTORY_HPP_
#define _PEAKENGINE_CORE_WORLDCOMPONENTFACTORY_HPP_

#include <string>

class TiXmlElement;

namespace peak
{
	class World;
	class WorldComponent;

	/**
	 * Class which produces world components.
	 */
	class WorldComponentFactory
	{
		public:
			/**
			 * Constructor.
			 * @param name Type name of the world components created by this
			 * factory.
			 * This name has to be unique for the Game the factory belongs to.
			 */
			WorldComponentFactory(std::string name) : name(name)
			{
			}
			virtual ~WorldComponentFactory()
			{
			}

			/**
			 * Returns the type name of the created components.
			 */
			std::string getName()
			{
				return name;
			}

			/**
			 * Creates a world component for the given world. Does not add
			 * it to the world.
			 */
			virtual WorldComponent *createComponent(World *world) = 0;
			/**
			 * Creates a world component from XML data. Does not add it to the
			 * world.
			 */
			virtual WorldComponent *createComponent(World *world,
				TiXmlElement *xml)
			{
				return createComponent(world);
			}
		private:
			std::string name;
	};
}

#endif
