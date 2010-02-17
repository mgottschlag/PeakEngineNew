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

#ifndef _PEAKENGINE_CORE_PROPERTY_HPP_
#define _PEAKENGINE_CORE_PROPERTY_HPP_

#include "../support/Buffer.hpp"

namespace peak
{
	class Entity;

	enum PropertyType
	{
		EPT_Int,
		EPT_Float,
		EPT_Vector2F,
		EPT_Vector2I,
		EPT_Vector3F,
		EPT_Vector3I,
		EPT_Quaternion,
		EPT_Quaternion16
	};

	class Property
	{
		public:
			Property(Entity *entity);
			virtual ~Property();

			virtual void serialize(Buffer *buffer) = 0;
			virtual void deserialize(Buffer *buffer) = 0;

			virtual bool hasChanged() = 0;

			void setDirty(bool dirty);
			bool isDirty();

			void setLastChange(unsigned int time);
			unsigned int getLastChange();

			virtual PropertyType getType() = 0;
		protected:
			void setChanged();
		private:
			bool dirty;
			Entity *entity;
			unsigned int changetime;
	};
}

#endif
