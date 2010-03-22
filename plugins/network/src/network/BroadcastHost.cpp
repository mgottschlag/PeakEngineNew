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

#include "peaknetwork/network/BroadcastHost.hpp"
#include "peakengine/support/Buffer.hpp"

#include <cstring>
#include <iostream>
#include <cerrno>

namespace peak
{
	namespace network
	{
		BroadcastHost::BroadcastHost() : reply(0)
		{
		}
		BroadcastHost::~BroadcastHost()
		{
			if (reply)
				reply->drop();
		}

		bool BroadcastHost::init(unsigned short port)
		{
			// Create raw socket
			// FIXME: This needs a really recent version of enet afaik.
			ENetAddress bcaddr;
			bcaddr.host = ENET_HOST_ANY;
			bcaddr.port = port;
			bcastsocket = enet_socket_create(ENET_SOCKET_TYPE_DATAGRAM);
			if (bcastsocket == -1)
				std::cout << "No socket." << std::endl;
			if (enet_socket_set_option(bcastsocket, ENET_SOCKOPT_REUSEADDR, 1))
				std::cout << "enet_socket_set_option1" << std::endl;
			if (enet_socket_bind(bcastsocket, &bcaddr))
				std::cout << "enet_socket_bind" << std::endl;
			if (enet_socket_set_option(bcastsocket, ENET_SOCKOPT_NONBLOCK, 1))
				std::cout << "enet_socket_set_option2" << std::endl;
			std::cout << "Started broadcast host (" << port << ")." << std::endl;
			return true;
		}
		bool BroadcastHost::shutdown()
		{
			enet_socket_destroy(bcastsocket);
			if (reply)
				reply->drop();
			reply = 0;
			return true;
		}

		void BroadcastHost::setReply(Buffer *reply)
		{
			if (this->reply)
				this->reply->drop();
			this->reply = reply;
			if (reply)
				reply->grab();
		}
		Buffer *BroadcastHost::getReply()
		{
			return reply;
		}

		void BroadcastHost::update()
		{
			std::cout << "BroadcastHost::update()" << std::endl;
			int length;
			ENetAddress addr;
			ENetBuffer bcbuffer;
			bcbuffer.data = new char[1024];
			bcbuffer.dataLength = 1024;
			// Look for broadcast messages
			while ((length = enet_socket_receive(bcastsocket, &addr, &bcbuffer, 1))
				!= 0)
			{
				if (length < 0)
				{
					std::cout << "Socket error: " << errno << std::endl;
					break;
				}
				else
				{
					std::cout << "Got broadcast message." << std::endl;
					// Send answer
					ENetBuffer replybuffer;
					if (reply)
					{
						replybuffer.data = reply->getData();
						replybuffer.dataLength = reply->getSize();
					}
					else
					{
						replybuffer.data = (void*)"PONG";
						replybuffer.dataLength = 5;
					}
					enet_socket_send(bcastsocket, &addr, &replybuffer, 1);
				}
			}
			delete[] (char*)bcbuffer.data;
		}
	}
}

