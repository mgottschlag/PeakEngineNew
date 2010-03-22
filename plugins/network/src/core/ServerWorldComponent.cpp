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

#include "peaknetwork/core/ServerWorldComponent.hpp"
#include "peaknetwork/core/ServerEntityComponent.hpp"
#include "peaknetwork/network/NetworkHost.hpp"
#include "peaknetwork/network/NetworkConnection.hpp"
#include "peaknetwork/network/NetworkData.hpp"
#include "peaknetwork/network/BroadcastHost.hpp"
#include "peakengine/core/World.hpp"
#include "peakengine/core/Entity.hpp"

#include <iostream>

namespace peak
{
	namespace network
	{
		unsigned int ClientInfo::lastid = 0;

		ServerWorldComponent::ServerWorldComponent(World *world)
			: NetworkWorldComponent(world, EWCT_Server), broadcast(0)
		{
		}
		ServerWorldComponent::~ServerWorldComponent()
		{
			if (broadcast)
			{
				broadcast->shutdown();
				delete broadcast;
			}
		}

		bool ServerWorldComponent::init(BufferPointer serverdata,
			unsigned int port, bool broadcast, unsigned int broadcastport)
		{
			// Create network host
			host = new NetworkHost();
			if (!host->init(port))
			{
				delete host;
				host = 0;
				return false;
			}
			// Save server data
			this->serverdata = serverdata;
			// Create broadcast host
			if (broadcast)
			{
				this->broadcast = new BroadcastHost();
				if (!this->broadcast->init(broadcastport))
				{
					delete host;
					host = 0;
					serverdata = 0;
					return false;
				}
			}
			return true;
		}

		void ServerWorldComponent::addEntity(Entity *entity)
		{
			ServerEntityComponent *component = (ServerEntityComponent*)entity->getComponent(EECT_Server);
			// Add entity to entity list
			if (freelist.empty())
			{
				// We don't have any free indices in the entity list
				entities.push_back(component);
				component->setID(entities.size());
			}
			else
			{
				// Reuse free index
				entities[freelist.front() - 1] = component;
				component->setID(freelist.front());
				freelist.pop();
			}
			// Save owner of the entity
			ClientInfo *owner = 0;
			unsigned int ownerid = component->getOwner();
			// Send the entity to all clients
			BufferPointer buffer = new Buffer();
			buffer->write8(EPT_EntityCreated);
			buffer->write16(component->getID());
			buffer->writeString(entity->getType());
			// Owner bit unset
			buffer->writeUnsignedInt(0, 1);
			component->getState(buffer.get());
			for (unsigned int i = 0; i < clients.size(); i++)
			{
				if (!clients[i].ready)
					continue;
				if (clients[i].id != ownerid)
					clients[i].connection->send(buffer, true);
				else
					owner = &clients[i];
			}
			// Send the entity to the owner
			if (!owner)
				return;
			buffer = new Buffer();
			buffer->write8(EPT_EntityCreated);
			buffer->write16(component->getID());
			buffer->writeString(entity->getType());
			// Set owner bit this time
			buffer->writeUnsignedInt(1, 1);
			component->getState(buffer.get());
			owner->connection->send(buffer, true);
		}
		void ServerWorldComponent::removeEntity(Entity *entity)
		{
			ServerEntityComponent *component = (ServerEntityComponent*)entity->getComponent(EECT_Server);
			// Remove the entity from the entity list
			unsigned int id = component->getID() - 1;
			if (!id >= entities.size())
				return;
			if (entities[id] != component)
				return;
			entities[id] = 0;
			freelist.push(id + 1);
			// Remove the entity from all clients
			BufferPointer buffer = new Buffer();
			buffer->write8(EPT_EntityDeleted);
			buffer->write16(component->getID() - 1);
			for (unsigned int i = 0; i < clients.size(); i++)
				clients[i].connection->send(buffer, true);
		}
		Entity *ServerWorldComponent::getEntity(unsigned int id)
		{
			ServerEntityComponent *component = getComponent(id);
			if (!component)
				return 0;
			return component->getEntity();
		}
		ServerEntityComponent *ServerWorldComponent::getComponent(unsigned int id)
		{
			id--;
			if (id >= entities.size())
				return 0;
			return entities[id];
		}

		void ServerWorldComponent::onPreUpdate()
		{
			host->update();
			if (broadcast)
				broadcast->update();
			// Incoming network connections
			NetworkConnection *newconnection = host->getNewConnection();
			while (newconnection)
			{
				std::cout << "Client connected!" << std::endl;
				insertNewConnection(newconnection);
				// Next connection
				newconnection = host->getNewConnection();
			}
			// Update clients
			for (unsigned int i = 0; i < clients.size(); i++)
			{
				// Increase client time. Might get overridden by any incoming
				// packet
				clients[i].clienttime++;
				// Receive data
				while (clients[i].connection->hasData())
				{
					BufferPointer data = clients[i].connection->receive();
					PacketType type = (PacketType)data->read8();
					switch (type)
					{
						case EPT_Update:
						{
							// Read last received packet
							clients[i].lastreceived = data->read32();
							unsigned int updatetime = data->read32();
							clients[i].clienttime = updatetime;
							// Read entity updates
							bool updatevalid = true;
							while (data->getPosition() + 16 <= data->getSize() * 8)
							{
								unsigned int id = data->read16() + 1;
								ServerEntityComponent *entity = (ServerEntityComponent*)getComponent(id);
								// Ignore invalid updates
								if (!entity)
								{
									updatevalid = false;
									break;
								}
								// Apply update
								entity->applyUpdate(data.get(), updatetime);
								entity->onUpdate();
							}
							// Only tell the client that we have received this
							// update if it was valid
							if (updatevalid)
								clients[i].lastupdate = updatetime;
							break;
						}
						case EPT_EntityMessage:
						{
							unsigned int id = data->read16() + 1;
							ServerEntityComponent *entity = getComponent(id);
							entity->onMessage().trigger(data.get());
							break;
						}
						case EPT_Ready:
						{
							// Send all existing entities
							// TODO: Inefficient, we should use much larger packets
							for (unsigned int j = 0; j < entities.size(); j++)
							{
								ServerEntityComponent *entity = entities[j];
								if (!entity)
									continue;
								BufferPointer buffer = new Buffer();
								buffer->write8(EPT_EntityCreated);
								buffer->write16(entity->getID());
								buffer->writeString(entity->getEntity()->getType());
								unsigned int owner = 0;
								if (clients[i].id == entity->getOwner())
									owner = 1;
								buffer->writeUnsignedInt(owner, 1);
								entity->getState(buffer.get());
								clients[i].connection->send(buffer, true);
							}
							// Activate updates
							clients[i].ready = true;
							break;
						}
						default:
							break;
					}
				}
			}
		}
		void ServerWorldComponent::onPostUpdate()
		{
			// Send updates
			for (unsigned int i = 0; i < clients.size(); i++)
			{
				if (!clients[i].ready)
					continue;
				// Create update buffer
				BufferPointer update = new Buffer();
				update->write8(EPT_Update);
				update->write32(getWorld()->getTime());
				update->write32(clients[i].clienttime);
				update->write32(clients[i].lastupdate);
				// Fill buffer with updates
				for (unsigned int j = 0; j < entities.size(); j++)
				{
					ServerEntityComponent *entity = entities[j];
					if (!entity)
						continue;
					if (entity->hasChanged(clients[i].lastreceived))
					{
						update->write16(entity->getID() - 1);
						entity->getUpdate(update.get(),
							clients[i].lastreceived);
					}
				}
				// Send update
				clients[i].connection->send(update);
			}
		}

		void ServerWorldComponent::insertNewConnection(NetworkConnection *connection)
		{
			// TODO: Check whether we want to accept the connection
			clients.push_back(ClientInfo(connection));
			// Write server data
			BufferPointer msg = new Buffer();
			msg->write8(EPT_InitialData);
			*msg.get() += *serverdata.get();
			connection->send(msg, true);
		}
	}
}
