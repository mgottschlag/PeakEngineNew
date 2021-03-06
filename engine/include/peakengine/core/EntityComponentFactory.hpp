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

#ifndef _PEAKENGINE_CORE_ENTITYCOMPONENTFACTORY_HPP_
#define _PEAKENGINE_CORE_ENTITYCOMPONENTFACTORY_HPP_

#include <string>

class TiXmlElement;

namespace peak
{
	class EntityComponent;
	class Entity;
	class EntityComponentFactory;

	class EntityComponentTemplate
	{
		public:
			EntityComponentTemplate(EntityComponentFactory *factory)
				: factory(factory)
			{
			}
			virtual ~EntityComponentTemplate()
			{
			}

			EntityComponentFactory *getFactory()
			{
				return factory;
			}
		private:
			EntityComponentFactory *factory;
	};

	/**
	 * Class which produces entity components.
	 */
	class EntityComponentFactory
	{
		public:
			/**
			 * Constructor.
			 * @param name Type name of the entity components created by this
			 * factory.
			 * This name has to be unique for the Game the factory belongs to.
			 */
			EntityComponentFactory(std::string name) : name(name)
			{
			}
			virtual ~EntityComponentFactory()
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
			 * Creates a component template from XML data. The caller has to
			 * delete the template when it is not needed anymore.
			 * @return Returns 0 if no template could be created.
			 */
			virtual EntityComponentTemplate *createTemplate(TiXmlElement *xml)
			{
				return 0;
			}

			/**
			 * Creates an entity component for the given entity. Does not add
			 * it to the entity.
			 */
			virtual EntityComponent *createComponent(Entity *entity) = 0;
			/**
			 * Creates an entity component for the given entity from XML data.
			 * Does not add it to the entity.
			 */
			virtual EntityComponent *createComponent(Entity *entity,
				EntityComponentTemplate *tpl)
			{
				return createComponent(entity);
			}
		private:
			std::string name;
	};
}

#endif
