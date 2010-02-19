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

#ifndef _PEAKENGINE_CORE_ENTITY_HPP_
#define _PEAKENGINE_CORE_ENTITY_HPP_

#include "../support/Mutex.hpp"

#include <string>
#include <map>
#include <vector>

namespace peak
{
	class World;
	class Property;
	class EntityComponent;

	/**
	 * Entity flags signalling the state of the entity. For example XML entities
	 * can load different components if different flags are set.
	 *
	 * For the flag bitmask, the value of the entries of this enum tells which
	 * bit is set (e.g. for EEF_Local the bit is (1 << EEF_Local)).
	 */
	enum EntityFlag
	{
		/**
		 * The entity is used on a game server.
		 */
		EEF_Server,
		/**
		 * The entity is used on a game client.
		 */
		EEF_Client,
		/**
		 * The entity is owned by this game instance (used with networking).
		 */
		EEF_Local,
		/**
		 * No entity flag, but rather the value returned by Entity::getFlag()
		 * if the string does not represent any of the flags.
		 */
		EEF_Invalid
	};

	/**
	 * Dynamic game object, this can be everything from the player to objects
	 * like doors or just a menu.
	 */
	class Entity
	{
		public:
			/**
			 * Constructor.
			 * @param world World into which this entity will get inserted
			 * later. This does not insert the entity into the world
			 * yet!
			 */
			Entity(World *world);
			/**
			 * Destructor.
			 */
			~Entity();

			/**
			 * Sets the ID of the entity. Entity IDs are unique within one
			 * world.
			 */
			void setID(unsigned int id);
			/**
			 * Returns the ID of the entity.
			 */
			unsigned int getID();

			/**
			 * Returns an entity flag.
			 */
			bool getFlag(EntityFlag flag);
			/**
			 * Sets an entity flag.
			 */
			void setFlag(EntityFlag flag, bool value);
			/**
			 * Returns an unsigned integer containing all flags as a bitset.
			 */
			unsigned int getFlags();
			/**
			 * Sets all flags to the value in the bitset.
			 */
			void setFlags(unsigned int flags);
			/**
			 * Returns the entity flag with the given name.
			 */
			static EntityFlag getFlag(std::string name);

			/**
			 * Sets the type name of the entity. This name is used e.g. for
			 * network replication.
			 */
			void setType(std::string type);
			/**
			 * Returns the type name of the entity.
			 */
			std::string getType()
			{
				return type;
			}

			/**
			 * Adds a property to the entity. Memory management for the
			 * property is then done by the entity.
			 * @return Property which is available under the name. Might be
			 * different from the property which was passed in if it already
			 * existed. Might be 0 if a property with the same name but
			 * different type already was registered.
			 */
			Property *addProperty(Property *property, std::string name);
			/**
			 * Returns the property with a certain name.
			 * @return 0 if no property with this name has been registered.
			 */
			Property *getProperty(std::string name);

			/**
			 * Adds a component to the entity. Memory management for the
			 * property is then done by the entity. Must not be called after
			 * init() has been called for this entity.
			 */
			void addComponent(EntityComponent *component);
			/**
			 * Returns the pointer to the n-th component with the given type,
			 * or 0 if there were no or too few components of this type.
			 */
			EntityComponent *getComponent(int type, unsigned int n);
			/**
			 * Returns the pointer to the first component with the given type,
			 * or 0 if there were no components of this type.
			 */
			EntityComponent *getComponent(int type);

			/**
			 * Returns the world this entity belongs to.
			 */
			World *getWorld()
			{
				return world;
			}

			/**
			 * Installs all components.
			 */
			bool init();

			/**
			 * Updates all components.
			 */
			void update();
		private:
			/**
			 * Type name.
			 */
			std::string type;
			/**
			 * World of the entity.
			 */
			World *world;
			/**
			 * ID of the entity.
			 */
			unsigned int id;
			/**
			 * Entity flags.
			 */
			unsigned int flags;
			/**
			 * Entity properties.
			 */
			std::map<std::string, Property*> properties;
			/**
			 * Components.
			 */
			std::vector<EntityComponent*> components;
			/**
			 * Mutex protecting the member variables.
			 */
			Mutex mutex;
	};
}

#endif
