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

#include "peaknetwork/core/NetworkEntityComponentFactory.hpp"
#include "peaknetwork/core/ServerEntityComponent.hpp"
#include "peaknetwork/core/ClientEntityComponent.hpp"
#include "peakengine/core/WorldComponent.hpp"
#include "peakengine/core/Entity.hpp"
#include "peakengine/core/World.hpp"

namespace peak
{
	namespace network
	{
		EntityComponent *NetworkEntityComponentFactory::createComponent(Entity *entity)
		{
			World *world = entity->getWorld();
			bool isserver = (world->getComponent(EWCT_Server) != 0);
			bool isclient = (world->getComponent(EWCT_Client) != 0);
			if (isserver)
				return new ServerEntityComponent(entity);
			else if (isclient)
				return new ClientEntityComponent(entity);
			else
				return 0;
		}
	}
}
