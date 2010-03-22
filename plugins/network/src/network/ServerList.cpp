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

#include "peaknetwork/network/ServerList.hpp"
#include "peakengine/support/Buffer.hpp"

namespace peak
{
	namespace network
	{
		ServerList::ServerList(const std::vector<std::string> &address,
			const std::vector<Buffer*> &serverinfo)
			: address(address), serverinfo(serverinfo)
		{
			for (unsigned int i = 0; i < serverinfo.size(); i++)
				serverinfo[i]->grab();
		}
		ServerList::~ServerList()
		{
			for (unsigned int i = 0; i < serverinfo.size(); i++)
				serverinfo[i]->drop();
		}

		unsigned int ServerList::getServerCount()
		{
			return address.size();
		}
		std::string ServerList::getAddress(unsigned int index)
		{
			if (index >= address.size())
				return "";
			return address[index];
		}
		Buffer *ServerList::getInfo(unsigned int index)
		{
			if (index >= serverinfo.size())
				return 0;
			return serverinfo[index];
		}
	}
}
