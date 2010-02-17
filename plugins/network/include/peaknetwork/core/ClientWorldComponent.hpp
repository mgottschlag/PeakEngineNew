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

#ifndef _PEAKNETWORK_CORE_CLIENTWORLDCOMPONENT_HPP_
#define _PEAKNETWORK_CORE_CLIENTWORLDCOMPONENT_HPP_

#include "NetworkWorldComponent.hpp"
#include "peakengine/support/Buffer.hpp"

#include <string>

namespace peak
{
	namespace network
	{
		class NetworkClient;
		class NetworkConnection;
		class ClientEntityComponent;

		class ClientWorldComponent : public NetworkWorldComponent
		{
			public:
				ClientWorldComponent(World *world);
				virtual ~ClientWorldComponent();

				bool init(std::string address, unsigned int port,
					unsigned int timeout = 5000);
				Buffer *getServerData()
				{
					return serverdata.get();
				}

				void setReady();

				virtual Entity *getEntity(unsigned int id);
				ClientEntityComponent *getComponent(unsigned int id);

				virtual void onPreUpdate();
				virtual void onPostUpdate();
			private:
				void addEntity(Entity *entity);
				void removeEntity(Entity *entity);

				BufferPointer serverdata;
				NetworkClient *client;
				NetworkConnection *connection;

				std::vector<ClientEntityComponent*> entities;

				unsigned int lastupdate;
				unsigned int lastacked;
		};
	}
}

#endif
