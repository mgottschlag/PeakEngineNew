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

#include "peaknetwork/core/ClientWorldComponent.hpp"
#include "peaknetwork/core/ClientEntityComponent.hpp"
#include "peaknetwork/network/NetworkClient.hpp"
#include "peaknetwork/network/NetworkConnection.hpp"
#include "peaknetwork/network/NetworkData.hpp"
#include "peakengine/core/World.hpp"
#include "peakengine/core/Engine.hpp"
#include "peakengine/core/Game.hpp"
#include "peakengine/core/Entity.hpp"

#include <iostream>

namespace peak
{
	namespace network
	{
		ClientWorldComponent::ClientWorldComponent(World *world)
			: NetworkWorldComponent(world, EWCT_Client), lastupdate(0),
			lastacked(0)
		{
		}
		ClientWorldComponent::~ClientWorldComponent()
		{
		}

		bool ClientWorldComponent::init(std::string address, unsigned int port,
			unsigned int timeout)
		{
			// Create connection
			client = new NetworkClient();
			if(!client->init())
			{
				delete client;
				client = 0;
				return false;
			}
			connection = client->connect(address, port, timeout);
			if(!connection)
			{
				delete client;
				client = 0;
				return false;
			}
			// Receive server data
			// FIXME: Infinite loop if the server does not respond.
			BufferPointer initialdata;
			while (!initialdata)
			{
				client->update();
				if (connection->hasData())
				{
					initialdata = connection->receive();
					PacketType type = (PacketType)initialdata->read8();
					if (type != EPT_InitialData)
					{
						initialdata = 0;
					}
				}
			}
			serverdata = initialdata;
			return true;
		}

		void ClientWorldComponent::setReady()
		{
			if (!connection)
				return;
			// Send ready signal
			BufferPointer msg = new Buffer();
			msg->write8(EPT_Ready);
			connection->send(msg, true);
		}

		Entity *ClientWorldComponent::getEntity(unsigned int id)
		{
			ClientEntityComponent *component = getComponent(id);
			if (!component)
				return 0;
			return component->getEntity();
		}
		ClientEntityComponent *ClientWorldComponent::getComponent(unsigned int id)
		{
			id--;
			if (id >= entities.size())
				return 0;
			return entities[id];
		}

		void ClientWorldComponent::onPreUpdate()
		{
			client->update();
			// Receive data
			while (connection->hasData())
			{
				BufferPointer data = connection->receive();
				PacketType type = (PacketType)data->read8();
				switch (type)
				{
					case EPT_EntityCreated:
					{
						unsigned int id = data->read16() + 1;
						std::string type = data->readString();
						bool local = data->readUnsignedInt(1);
						// Create entity
						Game *game = getWorld()->getEngine()->getGame();
						EntityFactory *factory = game->getEntityFactory(type);
						if (!factory)
						{
							std::cout << "Could not get entity factory " << type << std::endl;
							continue;
						}
						Entity *entity = factory->createEntity(getWorld());
						ClientEntityComponent *component = (ClientEntityComponent*)entity->getComponent(EECT_Client);
						component->setID(id);
						component->setState(data.get());
						addEntity(entity);
						getWorld()->addEntity(entity);
						break;
					}
					case EPT_EntityDeleted:
					{
						unsigned int id = data->read16() + 1;
						Entity *entity = getEntity(id);
						removeEntity(entity);
						delete entity;
						break;
					}
					case EPT_Update:
					{
						bool updatevalid = true;
						unsigned int updatetime = data->read32();
						unsigned int updateclienttime = data->read32();
						lastacked = data->read32();
						// Adjust time if the latency has decreased
						// TODO: This might affect client prediction
						if (updatetime > getWorld()->getTime())
							getWorld()->setTime(updatetime);
						// Apply the updates
						while (data->getPosition() + 16 <= data->getSize() * 8)
						{
							unsigned int id = data->read16() + 1;
							ClientEntityComponent *entity = getComponent(id);
							// Ignore invalid updates
							if (!entity)
							{
								updatevalid = false;
								break;
							}
							// Apply backup to remove any user changes
							if (entity->hasBackup())
								entity->applyBackup();
							// Apply update
							entity->applyUpdate(data.get(), updatetime);
							entity->onUpdate().trigger(updateclienttime);
						}
						// Only send the server that we have received this
						// update if it was valid
						if (updatevalid)
							lastupdate = updatetime;
						break;
					}
					case EPT_EntityMessage:
					{
						unsigned int id = data->read16() + 1;
						ClientEntityComponent *entity = getComponent(id);
						entity->onMessage().trigger(data.get());
						break;
					}
					default:
						// TODO: Warn
						break;
				}
			}
		}
		void ClientWorldComponent::onPostUpdate()
		{
			// Send update
			BufferPointer update = new Buffer();
			update->write8(EPT_Update);
			update->write32(lastupdate);
			update->write32(getWorld()->getTime());
			// Fill buffer with updates
			for (unsigned int i = 0; i < entities.size(); i++)
			{
				ClientEntityComponent *component = entities[i];
				if (component->hasChanged(lastacked))
				{
					update->write16(component->getID() - 1);
					component->getUpdate(update.get(), lastacked);
				}
			}
			connection->send(update);
		}

		void ClientWorldComponent::addEntity(Entity *entity)
		{
			ClientEntityComponent *component = (ClientEntityComponent*)entity->getComponent(EECT_Client);
			// Add the entity from the entity list
			unsigned int id = component->getID() - 1;
			// TODO: Inefficient?
			while (id > entities.size())
				entities.push_back(0);
			if (id == entities.size())
				entities.push_back(component);
			else
				entities[id] = component;
		}
		void ClientWorldComponent::removeEntity(Entity *entity)
		{
			ClientEntityComponent *component = (ClientEntityComponent*)entity->getComponent(EECT_Client);
			// Remove the entity from the entity list
			unsigned int id = component->getID() - 1;
			if (!id >= entities.size())
				return;
			if (entities[id] != component)
				return;
			entities[id] = 0;
		}
	}
}
