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

#include "peakengine/support/EngineScriptBinding.hpp"
#include "peakengine/support/ReferenceCounted.hpp"
#include "peakengine/support/Vector2.hpp"
#include "peakengine/support/Vector3.hpp"
#include "peakengine/support/Quaternion.hpp"
#include "peakengine/support/EventQueue.hpp"
#include "peakengine/core/Property.hpp"
#include "peakengine/core/Entity.hpp"
#include "peakengine/core/EntityComponent.hpp"
#include "peakengine/core/World.hpp"
#include "peakengine/core/IntProperty.hpp"
#include "peakengine/core/FloatProperty.hpp"
#include "peakengine/core/QuaternionProperty.hpp"
#include "peakengine/core/Vector2FProperty.hpp"
#include "peakengine/core/Vector3FProperty.hpp"
#include "peakengine/import/ScriptEntityComponent.hpp"

#include <luabind/operator.hpp>
#include <luabind/adopt_policy.hpp>

namespace peak
{
	template<class T> static T *get_pointer(const SharedPointer<T> &p)
	{
		return p.get();
	}

	template<class A> static SharedPointer<A const> *get_const_holder(SharedPointer<A>*)
	{
		return 0;
	}

	void EngineScriptBinding::apply(Script *script)
	{
		lua_State *state = script->getState();
		luabind::module(state, "peak")
		[
			// ReferenceCounted class
			luabind::class_<ReferenceCounted, SharedPointer<ReferenceCounted> >("ReferenceCounted")
				.def(luabind::constructor<>()),
			// Vector2F
			luabind::class_<Vector2F>("Vector2F")
				.def(luabind::constructor<>())
				.def(luabind::constructor<float, float>())
				.def(luabind::constructor<const char *>())
				.def(luabind::constructor<const Vector2F &>())
				.def(luabind::constructor<const Vector2I &>())
				.def("set", &Vector2F::set)
				.def("getLengthSquared", &Vector2F::getLengthSquared)
				.def("getLength", &Vector2F::getLength)
				.def("getRotation", &Vector2F::getRotation)
				.def("rotate", &Vector2F::rotate)
				.def(luabind::self * float())
				.def(luabind::self / float())
				.def(luabind::self + Vector2F())
				.def(luabind::self - Vector2F())
				.def(luabind::self + Vector2I())
				.def(luabind::self - Vector2I())
				.def(luabind::self == Vector2F())
				.def(luabind::self == Vector2I())
				.def_readwrite("x", &Vector2F::x)
				.def_readwrite("y", &Vector2F::y),
			// Vector2I
			luabind::class_<Vector2I>("Vector2I")
				.def(luabind::constructor<>())
				.def(luabind::constructor<float, float>())
				.def(luabind::constructor<const char *>())
				.def(luabind::constructor<const Vector2F &>())
				.def(luabind::constructor<const Vector2I &>())
				.def("set", &Vector2I::set)
				.def("getLengthSquared", &Vector2I::getLengthSquared)
				.def("getLength", &Vector2I::getLength)
				.def("getRotation", &Vector2I::getRotation)
				.def("rotate", &Vector2I::rotate)
				.def(luabind::self * float())
				.def(luabind::self / float())
				.def(luabind::self + Vector2F())
				.def(luabind::self - Vector2F())
				.def(luabind::self + Vector2I())
				.def(luabind::self - Vector2I())
				.def(luabind::self == Vector2F())
				.def(luabind::self == Vector2I())
				.def_readwrite("x", &Vector2I::x)
				.def_readwrite("y", &Vector2I::y),
			// Vector3F
			luabind::class_<Vector3F>("Vector3F")
				.def(luabind::constructor<>())
				.def(luabind::constructor<float, float, float>())
				.def(luabind::constructor<const Vector3F &>())
				.def(luabind::constructor<const Vector3I &>())
				.def("getLengthSquared", &Vector3F::getLengthSquared)
				.def("getLength", &Vector3F::getLength)
				.def("rotateXY", &Vector3F::rotateXY)
				.def("rotateXZ", &Vector3F::rotateXZ)
				.def("rotateYZ", &Vector3F::rotateYZ)
				.def("rotate", &Vector3F::rotate)
				.def("dot", &Vector3F::dot)
				.def("cross", &Vector3F::cross)
				.def("getAngle", &Vector3F::getAngle)
				.def("interpolate", &Vector3F::interpolate)
				.def(luabind::self * float())
				.def(luabind::self / float())
				.def(luabind::self + Vector3F())
				.def(luabind::self - Vector3F())
				.def(luabind::self == Vector3F())
				.def_readwrite("x", &Vector3F::x)
				.def_readwrite("y", &Vector3F::y)
				.def_readwrite("z", &Vector3F::z),
			// Quaternion
			luabind::class_<Quaternion>("Quaternion")
				.def(luabind::constructor<>())
				.def(luabind::constructor<float, float, float, float>())
				.def(luabind::constructor<const Quaternion &>())
				.def(luabind::constructor<const Vector3F &>()),
			// ScriptEventReceiver
			luabind::class_<ScriptEventReceiver>("ScriptEventReceiver")
				.def(luabind::constructor<Script*, std::string>())
				.def("getScript", &ScriptEventReceiver::getScript)
				.def("getFunction", &ScriptEventReceiver::getFunction),
			// EventReceiver
			luabind::class_<EventReceiver>("EventReceiver"),
			// EventReceiver2<int, int>
			luabind::class_<EventReceiver2<int, int> >("EventReceiver2II"),
			// Event
			luabind::class_<Event>("Event")
				.def(luabind::constructor<>())
				.def("connect", (void (Event::*)(EventReceiver*))&Event::connect)
				.def("disconnect", (void (Event::*)(EventReceiver*))&Event::disconnect)
				.def("connect", (void (Event::*)(ScriptEventReceiver*))&Event::connect)
				.def("disconnect", (void (Event::*)(ScriptEventReceiver*))&Event::disconnect)
				.def("trigger", &Event::trigger),
			// Event2II
			luabind::class_<Event2<int, int> >("Event2II")
				.def(luabind::constructor<>())
				.def("connect", (void (Event2<int, int>::*)(EventReceiver2<int, int>*))&Event2<int, int>::connect)
				.def("disconnect", (void (Event2<int, int>::*)(EventReceiver2<int, int>*))&Event2<int, int>::disconnect)
				.def("connect", (void (Event2<int, int>::*)(ScriptEventReceiver*))&Event2<int, int>::connect)
				.def("disconnect", (void (Event2<int, int>::*)(ScriptEventReceiver*))&Event2<int, int>::disconnect)
				.def("trigger", &Event2<int, int>::trigger),
			// EventQueue
			luabind::class_<EventQueue>("EventQueue")
				.def(luabind::constructor<>())
				.def("triggered", &EventQueue::triggered)
				.def("dropEvent", &EventQueue::dropEvent)
				.def("trigger", &EventQueue::trigger)
				.def("getReceiver", &EventQueue::getReceiver),
			// EventQueue2<int, int>
			luabind::class_<EventQueue2<int, int> >("EventQueue2II")
				.def(luabind::constructor<>())
				.def("triggered", &EventQueue2<int, int>::triggered)
				.def("dropEvent", &EventQueue2<int, int>::dropEvent)
				.def("getParam1", &EventQueue2<int, int>::getParam1)
				.def("getParam2", &EventQueue2<int, int>::getParam2)
				.def("trigger", &EventQueue2<int, int>::trigger)
				.def("getReceiver", &EventQueue2<int, int>::getReceiver),
			// Entity
			luabind::class_<Entity>("Entity")
				.def("getID", &Entity::getID)
				.def("getType", &Entity::getType)
				.def("addProperty", &Entity::addProperty, luabind::adopt(_2))
				.def("getProperty", &Entity::getProperty)
				.def("addComponent", &Entity::addComponent, luabind::adopt(_2))
				.def("getComponent", (EntityComponent *(Entity::*)(int))&Entity::getComponent)
				.def("getComponent", (EntityComponent *(Entity::*)(int, unsigned int))&Entity::getComponent)
				.def("getWorld", &Entity::getWorld),
			// EntityComponent
			luabind::class_<EntityComponent>("EntityComponent")
				.def("getType", &EntityComponent::getType)
				.def("getEntity", &EntityComponent::getEntity),
			// ScriptEntityComponent
			luabind::class_<ScriptEntityComponent, EntityComponent>("ScriptEntityComponent"),
			// Property
			luabind::class_<Property>("Property")
				.def("getType", &Property::getType),
			// IntProperty
			luabind::class_<IntProperty, Property>("IntProperty")
				.def(luabind::constructor<Entity*>())
				.def("init", &IntProperty::init)
				.def("get", &IntProperty::get)
				.def("set", &IntProperty::set)
				.def("getBit", &IntProperty::getBit)
				.def("setBit", &IntProperty::setBit),
			// FloatProperty
			luabind::class_<FloatProperty, Property>("FloatProperty")
				.def(luabind::constructor<Entity*>())
				.def("init", &FloatProperty::init)
				.def("get", &FloatProperty::get)
				.def("set", &FloatProperty::set),
			// QuaternionProperty
			luabind::class_<QuaternionProperty, Property>("QuaternionProperty")
				.def(luabind::constructor<Entity*>())
				.def("init", &QuaternionProperty::init)
				.def("get", &QuaternionProperty::get)
				.def("set", &QuaternionProperty::set),
			// QuaternionProperty16
			luabind::class_<QuaternionProperty16, Property>("QuaternionProperty16")
				.def(luabind::constructor<Entity*>())
				.def("init", &QuaternionProperty16::init)
				.def("get", &QuaternionProperty16::get)
				.def("set", &QuaternionProperty16::set),
			// Vector2FProperty
			luabind::class_<Vector2FProperty, Property>("Vector2FProperty")
				.def(luabind::constructor<Entity*>())
				.def("init", &Vector2FProperty::init)
				.def("get", &Vector2FProperty::get)
				.def("set", &Vector2FProperty::set),
			// Vector3FProperty
			luabind::class_<Vector3FProperty, Property>("Vector3FProperty")
				.def(luabind::constructor<Entity*>())
				.def("init", &Vector3FProperty::init)
				.def("get", &Vector3FProperty::get)
				.def("set", &Vector3FProperty::set)
		];
	}
}
