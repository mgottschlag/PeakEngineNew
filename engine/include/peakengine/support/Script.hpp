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

#ifndef _PEAKENGINE_SUPPORT_SCRIPT_HPP_
#define _PEAKENGINE_SUPPORT_SCRIPT_HPP_

#include "ScriptBinding.hpp"

#include <string>
extern "C"
{
	#include "lua.h"
}
#include <luabind/luabind.hpp>

namespace peak
{
	/**
	 * Lua script. Parts of the engine are exported into the lua VM, note
	 * however that you have to pass reference counted objects as shared
	 * pointers or the script will crash.
	 */
	class Script
	{
		public:
			/**
			 * Constructor.
			 */
			Script();
			/**
			 * Destructor.
			 */
			~Script();

			/**
			 * Executes a string containing lua code within the Lua VM.
			 */
			bool runString(std::string data);

			/**
			 * Returns true if a function with the given name exists in the
			 * global scope.
			 */
			bool isFunction(std::string name);
			/**
			 * Calls the function with the given name without any parameters.
			 */
			void callFunction(std::string name);

			// Functions without return value
			template <typename A> void callFunction(std::string name, A arg1)
			{
				luabind::call_function<void>(state, name.c_str(), arg1);
			};
			template <typename A1, typename A2> void callFunction(std::string name, A1 arg1, A2 arg2)
			{
				luabind::call_function<void>(state, name.c_str(), arg1, arg2);
			};
			template <typename A1, typename A2, typename A3> void callFunction(std::string name, A1 arg1, A2 arg2, A3 arg3)
			{
				luabind::call_function<void>(state, name.c_str(), arg1, arg2, arg3);
			};
			template <typename A1, typename A2, typename A3, typename A4> void callFunction(std::string name, A1 arg1, A2 arg2, A3 arg3, A4 arg4)
			{
				luabind::call_function<void>(state, name.c_str(), arg1, arg2, arg3, arg4);
			};

			// Functions with return value
			template <typename R> R callFunction(std::string name)
			{
				try
				{
					return luabind::call_function<R>(state, name.c_str());
				}
				catch (luabind::error &e)
				{
					std::string msg = lua_tostring(e.state(), -1);
					throw Exception(msg);
				}
			};
			template <typename R, typename A> R callFunction(std::string name, A arg1)
			{
				return luabind::call_function<R>(state, name.c_str(), arg1);
			};
			template <typename R, typename A1, typename A2> R callFunction(std::string name, A1 arg1, A2 arg2)
			{
				return luabind::call_function<R>(state, name.c_str(), arg1, arg2);
			};
			template <typename R, typename A1, typename A2, typename A3> R callFunction(std::string name, A1 arg1, A2 arg2, A3 arg3)
			{
				return luabind::call_function<R>(state, name.c_str(), arg1, arg2, arg3);
			};

			/**
			 * Sets a global variable to the given value. This can be any core
			 * type or any registered class. If the name contains dots, tables
			 * are created accordingly (e.g. "table.table2.variable").
			 */
			template <typename T> void setVariable(std::string name, T value)
			{
				luabind::object table = luabind::globals(state);
				while (name.find('.') != std::string::npos)
				{
					std::string tablename = name.substr(0, name.find('.'));
					name = name.substr(name.find('.') + 1);
					table = table[tablename.c_str()];
					if (luabind::type(table) != LUA_TTABLE)
						table = luabind::newtable(state);
				}
				table[name.c_str()] = value;
			};

			lua_State *getState()
			{
				return state;
			}

			/**
			 * Adds a script binding to the script. The ScriptBinding instance
			 * is not used in the script after this function call. The function
			 * just calls ScriptBinding::apply() internally.
			 */
			void addBinding(ScriptBinding *binding);

			class Exception: public std::runtime_error
			{
				public:
					Exception(std::string msg) : std::runtime_error(msg)
					{
					}
			};
		private:
			lua_State *state;
	};
}

#endif
