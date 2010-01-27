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

#include "peakengine/support/FrameLimiter.hpp"
#include "peakengine/support/OS.hpp"

namespace peak
{
	FrameLimiter::FrameLimiter(unsigned int interval)
		: interval(interval), lasttime(0)
	{
	}
	FrameLimiter::~FrameLimiter()
	{
	}

	void FrameLimiter::setInterval(unsigned int usecs)
	{
		interval = usecs;
	}
	unsigned int FrameLimiter::getInterval()
	{
		return interval;
	}

	void FrameLimiter::wait()
	{
		// Compute target time
		lasttime = lasttime + interval;
		// Get current time
		uint64_t currenttime = OS::getSystemTime();
		if (currenttime < lasttime)
		{
			// TODO: Statistics
			OS::sleep(lasttime - currenttime);
		}
		else
		{
			// TODO: Warning/statistics
		}
	}
}
