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

#include "peakengine/support/Script.hpp"

#include <iostream>
extern "C"
{
	#include "lualib.h"
	#include "lauxlib.h"
}
#include <luabind/operator.hpp>

namespace peak
{
	Script::Script()
	{
		state = lua_open();
		// Standard functions
		luaopen_base(state);
		luaopen_string(state);
		luaopen_math(state);
		luaopen_os(state);
		luabind::open(state);
	}
	Script::~Script()
	{
		lua_close(state);
	}

	bool Script::runString(std::string data)
	{
		int error = luaL_loadbuffer(state, data.c_str(), data.size(), "runString");
		if (!error)
		{
			error = lua_pcall(state, 0, LUA_MULTRET, 0);
		}
		if (error)
		{
			std::cerr << "Error while executing string:" << std::endl;
			std::cerr <<  lua_tostring(state, -1) << std::endl;
			std::cerr << "Code: \"" << data << "\"" << std::endl;
			return false;
		}
		return true;
	}

	bool Script::isFunction(std::string name)
	{
		lua_getglobal(state, name.c_str());
		bool exists = lua_isfunction(state, -1);
		lua_pop(state, 1);
		return exists;
	}

	void Script::callFunction(std::string name)
	{
		lua_getglobal(state, name.c_str());
		lua_call(state, 0, 0);
	}

	void Script::addBinding(ScriptBinding *binding)
	{
		binding->apply(this);
	}
}
