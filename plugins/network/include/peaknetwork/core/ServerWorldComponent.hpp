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

#ifndef _PEAKNETWORK_CORE_SERVERWORLDCOMPONENT_HPP_
#define _PEAKNETWORK_CORE_SERVERWORLDCOMPONENT_HPP_

#include "NetworkWorldComponent.hpp"
#include "peakengine/support/Buffer.hpp"

#include <string>
#include <queue>

namespace peak
{
	namespace network
	{
		class NetworkHost;
		class ServerEntityComponent;
		class ServerEntityComponent;
		class NetworkConnection;
		class BroadcastHost;

		class ClientInfo
		{
			public:
				ClientInfo(NetworkConnection *connection)
					: connection(connection), lastreceived(0), clienttime(0),
					lastupdate(0), ready(false)
				{
					id = ++lastid;
				}

				NetworkConnection *connection;
				unsigned int id;
				static unsigned int lastid;
				unsigned int lastreceived;
				unsigned int clienttime;
				unsigned int lastupdate;
				bool ready;
		};

		class ServerWorldComponent : public NetworkWorldComponent
		{
			public:
				ServerWorldComponent(World *world);
				virtual ~ServerWorldComponent();

				bool init(BufferPointer serverdata, unsigned int port = 27272,
					bool broadcast = false, unsigned int broadcastport = 27273);
				BufferPointer getServerData()
				{
					return serverdata.get();
				}

				void addEntity(Entity *entity);
				void removeEntity(Entity *entity);
				virtual Entity *getEntity(unsigned int id);
				ServerEntityComponent *getComponent(unsigned int id);

				virtual void onPreUpdate();
				virtual void onPostUpdate();

			private:
				void insertNewConnection(NetworkConnection *connection);

				BufferPointer serverdata;
				NetworkHost *host;
				std::vector<ClientInfo> clients;

				std::vector<ServerEntityComponent*> entities;
				std::queue<unsigned int> freelist;

				BroadcastHost *broadcast;
		};
	}
}

#endif
