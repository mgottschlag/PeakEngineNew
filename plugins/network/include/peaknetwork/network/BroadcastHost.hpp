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

#ifndef _PEAKNETWORK_NETWORK_BROADCASTHOST_HPP_
#define _PEAKNETWORK_NETWORK_BROADCASTHOST_HPP_

#include <enet/enet.h>
#include <string>

namespace peak
{
	class Buffer;

	namespace network
	{
		/**
		 * Socket which automatically replies to broadcast messages.
		 */
		class BroadcastHost
		{
			public:
				BroadcastHost();
				~BroadcastHost();

				/**
				 * Initializes the broadcast host.
				 * @param port Port on which the socket listens
				 */
				bool init(unsigned short port);
				/**
				 * Destroys the broadcast host.
				 */
				bool shutdown();

				/**
				 * Sets the reply of the broadcast host.
				 * @param info Reply data
				 */
				void setReply(Buffer *reply);
				/**
				 * Returns the used reply data.
				 */
				Buffer *getReply();

				/**
				 * Listens for incoming data and sends replies.
				 */
				void update();
			private:
				ENetSocket bcastsocket;

				Buffer *reply;
		};
	}
}

#endif

