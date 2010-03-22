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

#include "peaknetwork/network/BroadcastClient.hpp"
#include "peakengine/support/Buffer.hpp"

#include <cerrno>
#include <iostream>

namespace peak
{
	namespace network
	{
		BroadcastClient::BroadcastClient() : port(0)
		{
		}
		BroadcastClient::~BroadcastClient()
		{
			for (unsigned int i = 0; i < serverinfo.size(); i++)
				serverinfo[i]->drop();
		}

		void BroadcastClient::start(unsigned short port)
		{
			// Create raw socket
			socket = enet_socket_create(ENET_SOCKET_TYPE_DATAGRAM);
			if (enet_socket_set_option(socket, ENET_SOCKOPT_NONBLOCK, 1))
				std::cout << "enet_socket_set_option1" << std::endl;
			if (enet_socket_set_option(socket, ENET_SOCKOPT_BROADCAST, 1))
				std::cout << "enet_socket_set_option2" << std::endl;
			this->port = port;
			updatetime = 50;
			std::cout << "Started broadcast client (" << port << ")." << std::endl;
		}
		void BroadcastClient::stop()
		{
			enet_socket_destroy(socket);
			port = 0;
		}

		void BroadcastClient::clearList(void)
		{
			mutex.lock();
			address.clear();
			serverinfo.clear();
			mutex.unlock();
		}

		SharedPointer<ServerList> BroadcastClient::getServers()
		{
			mutex.lock();
			ServerList *list = new ServerList(address, serverinfo);
			mutex.unlock();
			return list;
		}

		void BroadcastClient::update()
		{
			// Broadcast address
			ENetAddress bcaddr;
			bcaddr.host = ENET_HOST_BROADCAST;
			bcaddr.port = port;
			// Message to be sent to all hosts
			char msg[] = "PING";
			ENetBuffer buffer;
			// Broadcast
			updatetime += 1;
			if (updatetime >= 50)
			{
				buffer.data = msg;
				buffer.dataLength = 5;
				std::cout << "Sending." << std::endl;
				if (enet_socket_send(socket, &bcaddr, &buffer, 1) < 5)
					perror("Sending failed");
				updatetime = 0;
			}
			// Create buffer for the replies
			buffer.data = malloc(1024);
			buffer.dataLength = 1024;
			ENetAddress remoteaddr;
			// Receive answers
			bool listchanged = false;
			int length;
			while ((length = enet_socket_receive(socket, &remoteaddr, &buffer, 1) > 0)
				!= 0)
			{
				if (length < 0)
				{
					std::cout << "Socket error: " << errno << std::endl;
					break;
				}
				if (length > 0)
				{
					std::cout << "Got broadcast response." << std::endl;
					// Get address
					char addrstr[16];
					enet_address_get_host_ip(&remoteaddr, addrstr, 16);
					// Get server info
					Buffer *reply = new Buffer(buffer.data, length);
					reply->grab();
					// Look if the server is already known
					bool unknown = true;
					for (unsigned int i = 0; i < address.size(); i++)
					{
						if (address[i] == addrstr)
						{
							unknown = false;
							// TODO: Compare data so that we do not issue too
							// many events.
							listchanged = true;
							serverinfo[i]->drop();
							serverinfo[i] = reply;
							break;
						}
					}
					// Add the server to the list
					if (unknown)
					{
						address.push_back(addrstr);
						serverinfo.push_back(reply);
						listchanged = true;
					}
				}
			}
			// Call calback function
			if (listchanged)
			{
				event.trigger();
			}

			free(buffer.data);
		}
	}
}

