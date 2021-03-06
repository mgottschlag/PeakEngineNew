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
#include "peakengine/support/ScreenPosition.hpp"
#include "peakengine/support/EventQueue.hpp"
#include "peakengine/support/Loadable.hpp"
#include "peakengine/support/Buffer.hpp"
#include "peakengine/core/Property.hpp"
#include "peakengine/core/Entity.hpp"
#include "peakengine/core/EntityComponent.hpp"
#include "peakengine/core/World.hpp"
#include "peakengine/core/WorldComponent.hpp"
#include "peakengine/core/Engine.hpp"
#include "peakengine/core/Game.hpp"
#include "peakengine/core/IntProperty.hpp"
#include "peakengine/core/FloatProperty.hpp"
#include "peakengine/core/QuaternionProperty.hpp"
#include "peakengine/core/Vector2FProperty.hpp"
#include "peakengine/core/Vector3FProperty.hpp"
#include "peakengine/import/ScriptEntityComponent.hpp"
#include "peakengine/import/XMLWorld.hpp"

#include <luabind/operator.hpp>
#include <luabind/adopt_policy.hpp>
#include <luabind/iterator_policy.hpp>

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
			// Loadable
			luabind::class_<Loadable, ReferenceCounted, SharedPointer<ReferenceCounted> >("Loadable"),
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
			// ScreenPosition
			luabind::class_<ScreenPosition>("ScreenPosition")
				.def(luabind::constructor<>())
				.def(luabind::constructor<Vector2F, Vector2I>())
				.def("getAbsolute", &ScreenPosition::getAbsolute)
				.def_readwrite("rel", &ScreenPosition::rel)
				.def_readwrite("abs", &ScreenPosition::abs)
				.def(luabind::self + ScreenPosition())
				.def(luabind::self - ScreenPosition()),
			// ScriptEventReceiver
			luabind::class_<ScriptEventReceiver>("ScriptEventReceiver")
				.def(luabind::constructor<Script*, std::string>())
				.def("getScript", &ScriptEventReceiver::getScript)
				.def("getFunction", &ScriptEventReceiver::getFunction),
			// EventReceiver
			luabind::class_<EventReceiver>("EventReceiver"),
			// EventReceiver1<int>
			luabind::class_<EventReceiver1<int> >("EventReceiver1I"),
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
			// Event1I
			luabind::class_<Event1<int> >("Event1I")
				.def(luabind::constructor<>())
				.def("connect", (void (Event1<int>::*)(EventReceiver1<int>*))&Event1<int>::connect)
				.def("disconnect", (void (Event1<int>::*)(EventReceiver1<int>*))&Event1<int>::disconnect)
				.def("connect", (void (Event1<int>::*)(ScriptEventReceiver*))&Event1<int>::connect)
				.def("disconnect", (void (Event1<int>::*)(ScriptEventReceiver*))&Event1<int>::disconnect)
				.def("trigger", &Event1<int>::trigger),
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
			// EventQueue1<int>
			luabind::class_<EventQueue1<int> >("EventQueue1I")
				.def(luabind::constructor<>())
				.def("triggered", &EventQueue1<int>::triggered)
				.def("dropEvent", &EventQueue1<int>::dropEvent)
				.def("getParam", &EventQueue1<int>::getParam)
				.def("trigger", &EventQueue1<int>::trigger)
				.def("getReceiver", &EventQueue1<int>::getReceiver),
			// Buffer
			luabind::class_<Buffer, ReferenceCounted, SharedPointer<ReferenceCounted> >("Buffer")
				.def(luabind::constructor<>())
				.def("setSize", &Buffer::setSize)
				.def("getSize", &Buffer::getSize)
				.def("setPosition", &Buffer::setPosition)
				.def("getPosition", &Buffer::getPosition)
				.def("write8", &Buffer::write8)
				.def("read8", &Buffer::read8)
				.def("write16", &Buffer::write16)
				.def("read16", &Buffer::read16)
				.def("write32", &Buffer::write32)
				.def("read32", &Buffer::read32)
				.def("write64", &Buffer::write64)
				.def("read64", &Buffer::read64)
				.def("writeFloat", &Buffer::writeFloat)
				.def("readFloat", &Buffer::readFloat)
				.def("writeString", &Buffer::writeString)
				.def("readString", &Buffer::readString)
				.def("writeInt", &Buffer::writeInt)
				.def("readInt", &Buffer::readInt)
				.def("writeUnsignedInt", &Buffer::writeUnsignedInt)
				.def("readUnsignedInt", &Buffer::readUnsignedInt)
				.def("nextByte", &Buffer::nextByte)
				.def("dump", &Buffer::dump),
			// Entity
			luabind::class_<Entity>("Entity")
				.def("getID", &Entity::getID)
				.def("getType", &Entity::getType)
				.def("addProperty", &Entity::addProperty, luabind::adopt(_2))
				.def("getProperty", &Entity::getProperty)
				.def("addComponent", &Entity::addComponent, luabind::adopt(_2))
				.def("getComponent", (EntityComponent *(Entity::*)(int))&Entity::getComponent)
				.def("getComponent", (EntityComponent *(Entity::*)(int, unsigned int))&Entity::getComponent)
				.def("getWorld", &Entity::getWorld)
				.def("getFlag", (bool (Entity::*)(EntityFlag))&Entity::getFlag)
				.def("getFlags", &Entity::getFlags)
				.def("setFlag", &Entity::setFlag)
				.enum_("EntityFlag")
				[
					luabind::value("ServerFlag", EEF_Server),
					luabind::value("ClientFlag", EEF_Client),
					luabind::value("LocalFlag", EEF_Local),
					luabind::value("InvalidFlag", EEF_Invalid)
				],
			// EntityComponent
			luabind::class_<EntityComponent>("EntityComponent")
				.def("getType", &EntityComponent::getType)
				.def("getEntity", &EntityComponent::getEntity)
				.enum_("EntityComponentType")
				[
					luabind::value("Script", EECT_Script),
					luabind::value("Physics", EECT_Physics),
					luabind::value("Graphics", EECT_Graphics),
					luabind::value("Server", EECT_Server),
					luabind::value("Client", EECT_Client)
				],
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
				.def("set", &Vector3FProperty::set),
			// World
			luabind::class_<World>("World")
				.def("getEngine", &World::getEngine)
				.def("getEntityCount", (unsigned int (World::*)())&World::getEntityCount)
				.def("getEntityCount", (unsigned int (World::*)(std::string))&World::getEntityCount)
				.def("getEntityCount", (std::vector<Entity*> (World::*)())&World::getEntities, luabind::return_stl_iterator)
				.def("getEntityCount", (std::vector<Entity*> (World::*)(std::string))&World::getEntities, luabind::return_stl_iterator)
				.def("getEntity", &World::getEntity)
				.def("addEntity", &World::addEntity)
				.def("removeEntity", &World::removeEntity)
				.def("addComponent", &World::addComponent, luabind::adopt(_2))
				.def("getComponent", &World::getComponent)
				.def("start", &World::start)
				.def("pause", &World::pause)
				.def("stop", &World::stop)
				.def("isRunning", &World::isRunning),
			// WorldComponent
			luabind::class_<WorldComponent>("WorldComponent")
				.def("setWorld", &WorldComponent::setWorld)
				.enum_("WorldComponentType")
				[
					luabind::value("Physics", EWCT_Physics),
					luabind::value("Graphics", EWCT_Graphics),
					luabind::value("Server", EWCT_Server),
					luabind::value("Client", EWCT_Client)
				],
			// Engine
			luabind::class_<Engine>("Engine")
				.def("stop", &Engine::stop)
				.def("getDirectory", &Engine::getDirectory)
				.def("getGame", &Engine::getGame)
				.def("addWorld", &Engine::addWorld, luabind::adopt(_2))
				.def("removeWorld", &Engine::removeWorld)
				.def("deleteWorld", &Engine::deleteWorld),
			// XMLWorld
			luabind::class_<XMLWorld, World>("XMLWorld")
				.def(luabind::constructor<Engine*, std::string>())
				.def("load", &XMLWorld::load),
			// Game
			luabind::class_<Game>("Game")
				.def("getEngine", &Game::getEngine)
				.def("getEntityFactory", &Game::getEntityFactory)
				.def("getEntityComponentFactory", &Game::getEntityComponentFactory)
				.def("getWorldComponentFactory", &Game::getWorldComponentFactory),
			// EntityFactory
			luabind::class_<EntityFactory>("EntityFactory")
				.def("getName", &EntityFactory::getName)
				.def("createEntity", &EntityFactory::createEntity)
		];
	}
}
