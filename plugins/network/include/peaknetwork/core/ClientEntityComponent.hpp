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

#ifndef _PEAKNETWORK_CORE_CLIENTENTITYCOMPONENT_HPP_
#define _PEAKNETWORK_CORE_CLIENTENTITYCOMPONENT_HPP_

#include "NetworkEntityComponent.hpp"

namespace peak
{
	namespace network
	{
		class ClientEntityComponent : public NetworkEntityComponent
		{
			public:
				ClientEntityComponent(Entity *entity);
				virtual ~ClientEntityComponent();

				bool hasChanged(unsigned int time);
				void getUpdate(Buffer *buffer, unsigned int time);
				void applyUpdate(Buffer *buffer, unsigned int time);
			private:
		};
	}
}

#endif
