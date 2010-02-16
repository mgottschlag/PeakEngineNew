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

#include "peaknetwork/core/NetworkEntityComponent.hpp"
#include "peakengine/core/Property.hpp"

namespace peak
{
	namespace network
	{
		NetworkEntityComponent::NetworkEntityComponent(Entity *entity)
			: EntityComponent(entity), id(0)
		{
		}
		NetworkEntityComponent::~NetworkEntityComponent()
		{
		}

		void NetworkEntityComponent::addProperty(Property *property)
		{
			properties.push_back(property);
		}
		void NetworkEntityComponent::addClientProperty(Property *property)
		{
			clientproperties.push_back(property);
		}

		void NetworkEntityComponent::setState(Buffer *buffer)
		{
			// Deserialize properties
			for (unsigned int i = 0; i < properties.size(); i++)
			{
				int changed = buffer->readUnsignedInt(1);
				if (changed)
					properties[i]->deserialize(buffer);
			}
			// Deserialize client properties
			for (unsigned int i = 0; i < clientproperties.size(); i++)
			{
				int changed = buffer->readUnsignedInt(1);
				if (changed)
					clientproperties[i]->deserialize(buffer);
			}
		}
		void NetworkEntityComponent::getState(Buffer *buffer)
		{
			// Serialize properties
			for (unsigned int i = 0; i < properties.size(); i++)
			{
				// Only save changed properties
				if (properties[i]->hasChanged())
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
			// Serialize client properties
			for (unsigned int i = 0; i < clientproperties.size(); i++)
			{
				// Only save changed properties
				if (clientproperties[i]->hasChanged())
				{
					// Bit set: Property changed.
					buffer->writeUnsignedInt(1, 1);
					// Write the property to the stream.
					clientproperties[i]->serialize(buffer);
				}
				else
				{
					// Bit not set: Property remained unchanged.
					buffer->writeUnsignedInt(0, 1);
				}
			}
		}
	}
}
