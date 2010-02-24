/*
Copyright (c) 2009, Mathias Gottschlag, Christian Reiser

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

#ifndef _PEAKPHYSICS_PHYSICS_CHARACTERCONTROLLER_HPP_
#define _PEAKPHYSICS_PHYSICS_CHARACTERCONTROLLER_HPP_

#include <peakengine/support/Vector3.hpp>

namespace peak
{
	namespace physics
	{
		class Simulation;
		class Body;
		class Shape;

		class CharacterController
		{
			public:
				CharacterController();
				~CharacterController();

				void init(Simulation *simulation);
				void destroy();

				void setHorizontalSpeed(Vector3F speed);
				bool onGround();

				void update();

				Body *getBody();
			private:
				Simulation *simulation;
				Body *body;
				Shape *shape;

				bool onground;
				Vector3F speed;
				Vector3F realspeed;
		};
	}
}

#endif