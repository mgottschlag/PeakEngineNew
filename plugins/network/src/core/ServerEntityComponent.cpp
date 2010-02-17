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

#include "peaknetwork/core/ServerEntityComponent.hpp"
#include "peaknetwork/core/ServerWorldComponent.hpp"
#include "peakengine/core/Property.hpp"
#include "peakengine/core/World.hpp"
#include "peakengine/core/Entity.hpp"

namespace peak
{
	namespace network
	{
		ServerEntityComponent::ServerEntityComponent(Entity *entity)
			: NetworkEntityComponent(entity), owner(0)
		{
		}
		ServerEntityComponent::~ServerEntityComponent()
		{
		}

		bool ServerEntityComponent::init()
		{
			World *world = getEntity()->getWorld();
			ServerWorldComponent *server = (ServerWorldComponent*)world->getComponent(EWCT_Server);
			if (!server)
				return false;
			server->addEntity(getEntity());
			return true;
		}

		bool ServerEntityComponent::hasChanged(unsigned int time)
		{
			for (unsigned int i = 0; i < properties.size(); i++)
			{
				if (properties[i]->getLastChange() > time)
					return true;
			}
			return false;
		}
		void ServerEntityComponent::getUpdate(Buffer *buffer, unsigned int time)
		{
			for (unsigned int i = 0; i < properties.size(); i++)
			{
				// TODO: Property flags
				if (properties[i]->getLastChange() > time)
				{
					// Bit set: Property changed.
					buffer->writeUnsignedInt(1, 1);
					// Write the property to the stream.
					properties[i]->serialize(buffer);
				}
				else
				{
					// Bit not set: Property remained unchanged.
					buffer->writeUnsignedInt(0, 1);
				}
			}
		}
		void ServerEntityComponent::applyUpdate(Buffer *buffer, unsigned int time)
		{
			// Update all properties.
			for (unsigned int i = 0; i < clientproperties.size(); i++)
			{
				int changed = buffer->readUnsignedInt(1);
				if (changed)
				{
					clientproperties[i]->deserialize(buffer);
				}
			}
			// TODO: Callback
		}
	}
}
