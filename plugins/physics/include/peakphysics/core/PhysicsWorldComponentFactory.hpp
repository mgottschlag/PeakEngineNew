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

#ifndef _PEAKPHYSICS_CORE_PHYSICSWORLDCOMPONENTFACTORY_HPP_
#define _PEAKPHYSICS_CORE_PHYSICSWORLDCOMPONENTFACTORY_HPP_

#include "peakengine/core/WorldComponentFactory.hpp"

namespace peak
{
	namespace physics
	{
		class Simulation;
		class Physics;

		class PhysicsWorldComponentFactory : public WorldComponentFactory
		{
			public:
				PhysicsWorldComponentFactory(Physics *physics);
				virtual ~PhysicsWorldComponentFactory();

				virtual WorldComponent *createComponent(World *world);
				virtual WorldComponent *createComponent(World *world,
					TiXmlElement *xml);
			private:
				Physics *physics;
		};
	}
}

#endif
