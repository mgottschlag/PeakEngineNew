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

#include "peakengine/support/Event.hpp"
#include "peakengine/support/EventQueue.hpp"

namespace peak
{
	ScriptEventReceiver::~ScriptEventReceiver()
	{
		for (int i = (int)events.size() - 1; i >= 0; i--)
		{
			events[i]->disconnect(this);
		}
	}

	EventReceiver::~EventReceiver()
	{
		for (int i = (int)events.size() - 1; i >= 0; i--)
		{
			events[i]->disconnect(this);
		}
		delete functor;
	}
}
