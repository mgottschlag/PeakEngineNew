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

#include "peaknetwork/core/NetworkScriptBinding.hpp"
#include "peaknetwork/core/Network.hpp"
#include "peaknetwork/core/ClientEntityComponent.hpp"
#include "peaknetwork/core/ServerEntityComponent.hpp"
#include "peaknetwork/core/ClientWorldComponent.hpp"
#include "peaknetwork/core/ServerWorldComponent.hpp"
#include "peaknetwork/network/NetworkConnection.hpp"
#include "peaknetwork/network/BroadcastHost.hpp"
#include "peaknetwork/network/BroadcastClient.hpp"
#include "peakengine/core/Engine.hpp"
#include "peakengine/core/Property.hpp"
#include "peakengine/core/World.hpp"
#include "peakengine/core/Entity.hpp"

#include <luabind/operator.hpp>
#include <luabind/adopt_policy.hpp>

namespace peak
{
	namespace network
	{
		template<class T> static T *get_pointer(const SharedPointer<T> &p)
		{
			return p.get();
		}

		template<class A> static SharedPointer<A const> *get_const_holder(SharedPointer<A>*)
		{
			return 0;
		}

		void NetworkScriptBinding::apply(Script *script)
		{
			lua_State *state = script->getState();
			luabind::module(state, "peak")
			[
				luabind::namespace_("network")
				[
					// Network
					luabind::class_<Network>("Network")
						.def("getEngine", &Network::getEngine),
					// NetworkEntityComponent
					luabind::class_<NetworkEntityComponent, EntityComponent>("NetworkEntityComponent")
						.def("getID", &NetworkEntityComponent::getID)
						.def("addProperty", &NetworkEntityComponent::addProperty)
						.def("addClientProperty", &NetworkEntityComponent::addClientProperty)
						.def("onUpdate", &NetworkEntityComponent::onUpdate)
						.def("onMessage", &NetworkEntityComponent::onMessage),
					// ServerEntityComponent
					luabind::class_<ServerEntityComponent, NetworkEntityComponent>("ServerEntityComponent")
						.def("setOwner", &ServerEntityComponent::setOwner)
						.def("getOwner", &ServerEntityComponent::getOwner),
					// ClientEntityComponent
					luabind::class_<ClientEntityComponent, NetworkEntityComponent>("ClientEntityComponent"),
					// NetworkWorldComponent
					luabind::class_<NetworkWorldComponent, WorldComponent>("NetworkWorldComponent")
						.def("getEntity", &NetworkWorldComponent::getEntity),
					// ServerWorldComponent
					luabind::class_<ServerWorldComponent, NetworkWorldComponent>("ServerWorldComponent")
						.def(luabind::constructor<World*>())
						.def("init", &ServerWorldComponent::init)
						.def("getServerData", &ServerWorldComponent::getServerData)
						.def("addEntity", &ServerWorldComponent::addEntity)
						.def("removeEntity", &ServerWorldComponent::removeEntity)
						.def("getConnectionEvent", &ServerWorldComponent::getConnectionEvent),
					// ClientWorldComponent
					luabind::class_<ClientWorldComponent, NetworkWorldComponent>("ClientWorldComponent")
						.def(luabind::constructor<World*>())
						.def("init", &ClientWorldComponent::init)
						.def("getServerData", &ClientWorldComponent::getServerData),
					// ServerList
					luabind::class_<ServerList, ReferenceCounted, SharedPointer<ReferenceCounted> >("ServerList")
						.def("getServerCount", &ServerList::getServerCount)
						.def("getAddress", &ServerList::getAddress)
						.def("getInfo", &ServerList::getInfo),
					// BroadcastHost
					luabind::class_<BroadcastHost>("BroadcastHost")
						.def(luabind::constructor<>())
						.def("init", &BroadcastHost::init)
						.def("shutdown", &BroadcastHost::shutdown)
						.def("setReply", &BroadcastHost::setReply)
						.def("getReply", &BroadcastHost::getReply)
						.def("update", &BroadcastHost::update),
					// BroadcastClient
					luabind::class_<BroadcastClient>("BroadcastClient")
						.def(luabind::constructor<>())
						.def("start", &BroadcastClient::start)
						.def("stop", &BroadcastClient::stop)
						.def("clearList", &BroadcastClient::clearList)
						.def("getServers", &BroadcastClient::getServers)
						.def("getEvent", &BroadcastClient::getEvent)
						.def("update", &BroadcastClient::update)
				]
			];
		}
	}
}
