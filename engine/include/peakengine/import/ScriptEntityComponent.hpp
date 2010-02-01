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

#ifndef _PEAKENGINE_IMPORT_SCRIPTENTITYCOMPONENT_HPP_
#define _PEAKENGINE_IMPORT_SCRIPTENTITYCOMPONENT_HPP_

#include "../core/EntityComponent.hpp"

#include <string>

namespace peak
{
	class Script;

	class ScriptEntityComponent : public EntityComponent
	{
		public:
			ScriptEntityComponent(Entity *entity, std::string data);
			virtual ~ScriptEntityComponent();

			virtual bool installProperties();
			virtual bool init();

			virtual void update();

			virtual int getType();
		private:
			Script *script;
	};
}

#endif
