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

#ifndef _PEAKENGINE_CORE_XMLENTITYFACTORY_HPP_
#define _PEAKENGINE_CORE_XMLENTITYFACTORY_HPP_

#include "../core/EntityFactory.hpp"

#include "peakengine/support/tinyxml.h"

#include <vector>

namespace peak
{
	class EntityComponentTemplate;

	class XMLEntityFactory : public EntityFactory
	{
		public:
			XMLEntityFactory(std::string name, std::string file);
			virtual ~XMLEntityFactory();

			bool load();

			virtual Entity *createEntity(World *world, unsigned int flags = 0);
		private:
			std::string file;
			TiXmlDocument xml;
			struct ComponentInfo
			{
				std::string type;
				bool loaded;
				TiXmlElement *xml;
				EntityComponentTemplate *tpl;
				unsigned int flagsset;
				unsigned int flagsunset;
			};
			std::vector<ComponentInfo> components;
	};
}

#endif
