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

namespace peak
{
	namespace network
	{
		ClientWorldComponent::ClientWorldComponent(World *world)
			: NetworkWorldComponent(world, EWCT_Client)
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
		void ClientWorldComponent::onPreUpdate()
		{
		}
		void ClientWorldComponent::onPostUpdate()
		{
		}
	}
}
