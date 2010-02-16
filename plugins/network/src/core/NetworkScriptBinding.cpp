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
#include "peaknetwork/network/NetworkConnection.hpp"
#include "peakengine/core/Engine.hpp"

#include <luabind/operator.hpp>

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
				luabind::namespace_("graphics")
				[
					// Network
					luabind::class_<Network>("Network")
						.def("getEngine", &Network::getEngine)
				]
			];
		}
	}
}