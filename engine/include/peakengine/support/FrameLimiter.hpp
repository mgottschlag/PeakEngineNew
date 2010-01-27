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

#ifndef _PEAKENGINE_SUPPORT_FRAMELIMITER_HPP_
#define _PEAKENGINE_SUPPORT_FRAMELIMITER_HPP_

#include "SizedIntegers.hpp"

namespace peak
{
	/**
	 * Class which ensures that a loop (e.g. world update loop) is only run a
	 * certain number of times per second. Note that the interval is set in
	 * microseconds, but you will never get that accuracy - on Windows for
	 * example the times can vary for about 16 milliseconds. The FrameLimiter
	 * class will try to ensure a certain number of iterations per second
	 * though, e.g. if the OS timer waits 30 instead of 20 ms once, the class
	 * will only wait 10 ms the next time wait() is called. As soon as one slice
	 * of time is used up, e.g. the OS timer waited 40 ms, the class will drop
	 * 20 ms and not let wait return instantly twice.
	 * @note Not threadsafe.
	 */
	class FrameLimiter
	{
		public:
			/**
			 * Constructor.
			 * @param interval Interval of the timer in microseconds.
			 */
			FrameLimiter(unsigned int interval = 20000);
			/**
			 * Destructor.
			 */
			~FrameLimiter();

			/**
			 * Sets the interval (in microseconds).
			 */
			void setInterval(unsigned int usecs);
			/**
			 * Returns the interval.
			 */
			unsigned int getInterval();

			/**
			 * If between the last call of this function and this call is no
			 * more time than the interval set for this instance, the function
			 * will wait until the interval was used up. If the OS timer happens
			 * to wait longer than requested, the difference is substracted from
			 * the time for the next wait() call.
			 */
			void wait();
		private:
			unsigned int interval;
			uint64_t lasttime;
	};
}

#endif
