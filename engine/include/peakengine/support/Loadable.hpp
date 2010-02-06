/*
Copyright (c) 2009, Mathias Gottschlag

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

#ifndef _PEAKGRAPHICS_SUPPORT_LOADABLE_HPP_
#define _PEAKGRAPHICS_SUPPORT_LOADABLE_HPP_

#include "ReferenceCounted.hpp"

namespace peak
{
	class Loadable : public ReferenceCounted
	{
		public:
			Loadable() : ReferenceCounted(), loaded(false)
			{
			}
			virtual ~Loadable()
			{
			}

			bool tryLoading()
			{
				if (load())
				{
					setLoaded(true);
					return true;
				}
				else
					return false;
			}
			virtual bool load() = 0;
			virtual bool destroy()
			{
				return false;
			}
			void setLoaded(bool loaded)
			{
				this->loaded = loaded;
			}
			bool isLoaded()
			{
				return loaded;
			}
		private:
			bool loaded;
	};
}

#endif
