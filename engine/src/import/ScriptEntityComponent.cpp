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

#include "peakengine/import/ScriptEntityComponent.hpp"
#include "peakengine/support/Script.hpp"
#include "peakengine/support/EngineScriptBinding.hpp"
#include "peakengine/core/Game.hpp"
#include "peakengine/core/Entity.hpp"
#include "peakengine/core/World.hpp"
#include "peakengine/core/Engine.hpp"

#include <iostream>

namespace peak
{
	ScriptEntityComponent::ScriptEntityComponent(Entity *entity, std::string data)
		: EntityComponent(entity)
	{
		// Create script
		script = new Script();
		// Bindings
		EngineScriptBinding enginebinding;
		script->addBinding(&enginebinding);
		Game *game = entity->getWorld()->getEngine()->getGame();
		const std::vector<ScriptBinding*> &bindings = game->getScriptBindings();
		for (unsigned int i = 0; i < bindings.size(); i++)
		{
			script->addBinding(bindings[i]);
		}
		// Register component
		script->setVariable("this", this);
		// Execute script
		if (!script->runString(data))
		{
			std::cout << "Error while running script: \"" << std::endl;
			std::cout << "--------------------------" << std::endl;
			std::cout << data << std::endl;
			std::cout << "--------------------------" << std::endl;
		}
	}
	ScriptEntityComponent::~ScriptEntityComponent()
	{
		delete script;
	}

	bool ScriptEntityComponent::installProperties()
	{
		if (!script->isFunction("installProperties"))
			return false;
		return script->callFunction<bool>("installProperties");
	}
	bool ScriptEntityComponent::init()
	{
		if (!script->isFunction("init"))
			return false;
		return script->callFunction<bool>("init");
	}
	void ScriptEntityComponent::destroy()
	{
		if (script->isFunction("destroy"))
			script->callFunction("destroy");
	}

	void ScriptEntityComponent::update()
	{
		if (script->isFunction("update"))
			script->callFunction("update");
	}

	int ScriptEntityComponent::getType()
	{
		return EECT_Script;
	}
}
