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

#ifndef _PEAKNETWORK_NETWORK_BROADCASTCLIENT_HPP_
#define _PEAKNETWORK_NETWORK_BROADCASTCLIENT_HPP_

#include <enet/enet.h>
#include <string>
#include <vector>

#include "ServerList.hpp"
#include "peakengine/support/Event.hpp"
#include "peakengine/support/Mutex.hpp"

namespace peak
{
	namespace network
	{
		/**
		 * Socket which sends broadcast messages at a fixed interval to get the
		 * address of local network servers.
		 */
		class BroadcastClient
		{
			public:
				BroadcastClient();
				~BroadcastClient();

				/**
				 * Starts broadcasting.
				 * @param port Port to send messages at
				 */
				void start(unsigned short port);
				/**
				 * Stops broadcasting
				 */
				void stop(void);

				/**
				 * Clears the server list.
				 */
				void clearList(void);

				/**
				 * Returns a list with the number of servers found in the local
				 * network.
				 */
				SharedPointer<ServerList> getServers();

				/**
				 * Returns an event which is called whenever the internal server
				 * list changes. getServers() can then be called to get the
				 * updated server list.
				 */
				Event &getEvent()
				{
					return event;
				}

				/**
				 * Sends a broadcast message if needed, handles incoming
				 * responses. This is supposed to be called 50 times a second.
				 * If it is called at a different rate, the update frequency
				 * might vary.
				 */
				void update();
			private:
				ENetSocket socket;
				unsigned short port;

				unsigned int updatetime;

				std::vector<std::string> address;
				std::vector<Buffer*> serverinfo;

				Event event;
				Mutex mutex;
		};
	}
}

#endif

