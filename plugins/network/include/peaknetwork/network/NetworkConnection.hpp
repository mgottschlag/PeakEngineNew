/*
Copyright (c) 2009, Lukas Kropatschek

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

#ifndef _PEAKNETWORK_NETWORK_NETWORKCONNECTION_HPP_
#define _PEAKNETWORK_NETWORK_NETWORKCONNECTION_HPP_

#include "peakengine/support/Buffer.hpp"

#include <enet/enet.h>
#include <queue>

namespace peak
{
	namespace network
	{
		class NetworkConnection
		{
			public:
				NetworkConnection(ENetPeer* peer);
				~NetworkConnection();

				void injectData(BufferPointer buffer);

				bool isConnected();
				void close();

				void send(BufferPointer buffer, bool reliable = false);
				bool hasData();
				BufferPointer receive();
			private:
				std::queue<BufferPointer> received;
				ENetPeer* peer;
		};
	}
}

#endif
