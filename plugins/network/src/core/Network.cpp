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

#include "peaknetwork/core/Network.hpp"
#include "peaknetwork/core/NetworkEntityComponentFactory.hpp"
#include "peaknetwork/core/NetworkScriptBinding.hpp"
#include "peakengine/core/Game.hpp"

#include <enet/enet.h>

namespace peak
{
	namespace network
	{
		Network::Network() : engine(0)
		{
			// Initialize networking
			enet_initialize();
		}
		Network::~Network()
		{
			// Shutdown networking
			enet_deinitialize();
		}

		void Network::registerComponents(Game *game)
		{
			// Register components
			NetworkEntityComponentFactory *factory = new NetworkEntityComponentFactory(this);
			game->addEntityComponentFactory(factory);
			// Register script bindings
			game->addScriptBinding(new NetworkScriptBinding());
		}
	}
}
