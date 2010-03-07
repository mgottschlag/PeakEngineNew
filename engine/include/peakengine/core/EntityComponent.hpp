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

#ifndef _PEAKENGINE_CORE_ENTITYCOMPONENT_HPP_
#define _PEAKENGINE_CORE_ENTITYCOMPONENT_HPP_

namespace peak
{
	class Entity;

	enum EntityComponentType
	{
		EECT_Script = 1,
		EECT_Physics = 2,
		EECT_Graphics = 3,
		EECT_Server = 4,
		EECT_Client = 5
	};

	class EntityComponent
	{
		public:
			EntityComponent(Entity *entity);
			virtual ~EntityComponent();

			virtual bool installProperties();
			virtual bool init();
			virtual void destroy();

			virtual void update();

			virtual int getType() = 0;

			Entity *getEntity()
			{
				return entity;
			}
		private:
			Entity *entity;
	};
}

#endif

