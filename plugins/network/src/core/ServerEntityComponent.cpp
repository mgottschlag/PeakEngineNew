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

#include "peaknetwork/core/ServerEntityComponent.hpp"

namespace peak
{
	namespace network
	{
		ServerEntityComponent::ServerEntityComponent(Entity *entity)
			: NetworkEntityComponent(entity), owner(0)
		{
		}
		ServerEntityComponent::~ServerEntityComponent()
		{
		}

		bool ServerEntityComponent::hasChanged(unsigned int time)
		{
		}
		void ServerEntityComponent::getUpdate(Buffer *buffer, unsigned int time)
		{
		}
		void ServerEntityComponent::applyUpdate(Buffer *buffer, unsigned int time)
		{
		}
	}
}
