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

#ifndef _PEAKENGINE_CORE_WORLDCOMPONENT_HPP_
#define _PEAKENGINE_CORE_WORLDCOMPONENT_HPP_

namespace peak
{
	class World;

	enum WorldComponentType
	{
		EWCT_Server,
		EWCT_Client,
		EWCT_Graphics,
		EWCT_Physics,
		EWCT_Sound
	};

	class WorldComponent
	{
		public:
			WorldComponent(World *world, unsigned int type)
				: world(world), type(type)
			{
			}
			virtual ~WorldComponent()
			{
			}

			virtual void onPostUpdate() {};
			virtual void onPreUpdate() {};

			unsigned int getType()
			{
				return type;
			}
			void setWorld(World *world)
			{
				this->world = world;
			}
			World *getWorld()
			{
				return world;
			}
		private:
			World *world;
			unsigned int type;
	};
}

#endif
