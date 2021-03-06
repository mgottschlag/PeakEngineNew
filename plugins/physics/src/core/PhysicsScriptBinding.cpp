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

#include "peakphysics/core/PhysicsScriptBinding.hpp"
#include "peakphysics/core/PhysicsEntityComponent.hpp"
#include "peakphysics/physics/Body.hpp"
#include "peakphysics/physics/CharacterController.hpp"
#include "peakphysics/physics/Simulation.hpp"

#include <luabind/adopt_policy.hpp>
#include <luabind/out_value_policy.hpp>

namespace peak
{
	namespace physics
	{
		void PhysicsScriptBinding::apply(Script *script)
		{
			lua_State *state = script->getState();
			luabind::module(state, "peak")
			[
				luabind::namespace_("physics")
				[
					// PhysicsEntityComponent
					luabind::class_<PhysicsEntityComponent, EntityComponent>("PhysicsEntityComponent")
						.def("addBody", &PhysicsEntityComponent::addBody, luabind::adopt(_3))
						.def("getBody", &PhysicsEntityComponent::getBody)
						.def("addCharacterController", &PhysicsEntityComponent::addCharacterController, luabind::adopt(_3))
						.def("getCharacterController", &PhysicsEntityComponent::getCharacterController)
						.def("getSimulation", &PhysicsEntityComponent::getSimulation),
					// Body
					luabind::class_<Body>("Body")
						.def("setPosition", &Body::setPosition)
						.def("getPosition", &Body::getPosition)
						.def("setRotation",(void (Body::*)(const Quaternion&)) &Body::setRotation)
						.def("setRotation",(void (Body::*)(const Vector3F&)) &Body::setRotation)
						.def("getRotation", &Body::getRotation),
					// CharacterController
					luabind::class_<CharacterController>("CharacterController")
						.def(luabind::constructor<>())
						.def("init", &CharacterController::init)
						.def("destroy", &CharacterController::destroy)
						.def("setHorizontalSpeed", &CharacterController::setHorizontalSpeed)
						.def("onGround", &CharacterController::onGround)
						.def("update", &CharacterController::update)
						.def("getBody", &CharacterController::getBody),
					// CollisionInfo
					luabind::class_<CollisionInfo>("CollisionInfo")
						.def(luabind::constructor<>())
						.def_readwrite("lambda", &CollisionInfo::lambda)
						.def_readwrite("point", &CollisionInfo::point)
						.def_readwrite("body", &CollisionInfo::body),
					// Simulation
					luabind::class_<Simulation>("Simulation")
						.def("castRay", &Simulation::castRay, luabind::pure_out_value(_4))
				]
			];
		}
	}
}
