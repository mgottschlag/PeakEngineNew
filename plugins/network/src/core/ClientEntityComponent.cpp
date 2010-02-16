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

#include "peaknetwork/core/ClientEntityComponent.hpp"
#include "peakengine/core/Property.hpp"

namespace peak
{
	namespace network
	{
		ClientEntityComponent::ClientEntityComponent(Entity *entity)
			: NetworkEntityComponent(entity), backupvalid(false)
		{
		}
		ClientEntityComponent::~ClientEntityComponent()
		{
		}

		bool ClientEntityComponent::hasChanged(unsigned int time)
		{
			for (unsigned int i = 0; i < clientproperties.size(); i++)
			{
				if (clientproperties[i]->getLastChange() > time)
					return true;
			}
			return false;
		}
		void ClientEntityComponent::getUpdate(Buffer *buffer, unsigned int time)
		{
			for (unsigned int i = 0; i < clientproperties.size(); i++)
			{
				// TODO: Property flags
				if (clientproperties[i]->getLastChange() > time)
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
		void ClientEntityComponent::applyUpdate(Buffer *buffer, unsigned int time)
		{
			// Update all properties.
			for (unsigned int i = 0; i < properties.size(); i++)
			{
				int changed = buffer->readUnsignedInt(1);
				if (changed)
				{
					properties[i]->deserialize(buffer);
				}
			}
			// TODO: Callback
		}

		void ClientEntityComponent::createBackup()
		{
			backupvalid = true;
			// Create backup buffer
			if (!backup)
			{
				backup = new Buffer();
			}
			else
			{
				backup->setPosition(0);
			}
			// Store all properties
			// TODO: This is still slow in many cases where packing causes overhead
			for (unsigned int i = 0; i < properties.size(); i++)
			{
				properties[i]->serialize(backup.get());
			}
		}
		bool ClientEntityComponent::hasBackup()
		{
		return backupvalid;
		}
		void ClientEntityComponent::applyBackup()
		{
			if (!backupvalid)
				return;
			// Reload all properties
			backup->setPosition(0);
			for (unsigned int i = 0; i < properties.size(); i++)
			{
				properties[i]->deserialize(backup.get());
			}
		}
		void ClientEntityComponent::discardBackup()
		{
			backupvalid = false;
		}
	}
}
