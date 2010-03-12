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

#include "peakphysics/core/PhysicsEntityComponent.hpp"
#include "peakphysics/physics/Body.hpp"
#include "peakphysics/physics/CharacterController.hpp"

namespace peak
{
	namespace physics
	{
		PhysicsEntityComponent::PhysicsEntityComponent(Entity *entity,
			Simulation *simulation) : EntityComponent(entity),
			simulation(simulation)
		{
		}
		PhysicsEntityComponent::~PhysicsEntityComponent()
		{
			// Delete bodies
			std::map<std::string, Body*>::iterator it = bodies.begin();
			while (it != bodies.end())
			{
				// TODO: Delete shapes
				it->second->destroy();
				delete it->second;
				it++;
			}
			// Delete character controllers
			std::map<std::string, CharacterController*>::iterator it2 = charcontrollers.begin();
			while (it2 != charcontrollers.end())
			{
				// TODO: Delete shapes
				it2->second->destroy();
				delete it2->second;
				it2++;
			}
		}

		void PhysicsEntityComponent::addBody(std::string name, Body *body)
		{
			bodies[name] = body;
		}
		Body *PhysicsEntityComponent::getBody(std::string name)
		{
			std::map<std::string, Body*>::iterator it = bodies.find(name);
			if (it != bodies.end())
				return it->second;
			return 0;
		}

		void PhysicsEntityComponent::addCharacterController(std::string name, CharacterController *c)
		{
			charcontrollers[name] = c;
		}
		CharacterController *PhysicsEntityComponent::getCharacterController(std::string name)
		{
			std::map<std::string, CharacterController*>::iterator it = charcontrollers.find(name);
			if (it != charcontrollers.end())
				return it->second;
			return 0;
		}

		void PhysicsEntityComponent::update()
		{
			for (std::map<std::string, CharacterController*>::iterator it = charcontrollers.begin();
				it != charcontrollers.end(); it++)
			{
				it->second->update();
			}
		}
	}
}
