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

#ifndef _PEAKNETWORK_CORE_NETWORKENTITYCOMPONENT_HPP_
#define _PEAKNETWORK_CORE_NETWORKENTITYCOMPONENT_HPP_

#include "peakengine/core/EntityComponent.hpp"
#include "peakengine/support/Event.hpp"
#include "peakengine/support/Buffer.hpp"

namespace peak
{
	class Property;

	namespace network
	{
		class NetworkEntityComponent : public EntityComponent
		{
			public:
				NetworkEntityComponent(Entity *entity);
				virtual ~NetworkEntityComponent();

				void setID(unsigned int id)
				{
					this->id = id;
				}
				unsigned int getID()
				{
					return id;
				}

				void addProperty(Property *property);
				void addClientProperty(Property *property);

				void setState(Buffer *buffer);
				void getState(Buffer *buffer);

				Event1<unsigned int> &onUpdate()
				{
					return onupdate;
				}
				Event1<Buffer*> &onMessage()
				{
					return onmessage;
				}
			private:
				Event1<unsigned int> onupdate;
				Event1<Buffer*> onmessage;

				std::vector<Property*> properties;
				std::vector<Property*> clientproperties;

				unsigned int id;
		};
	}
}

#endif
