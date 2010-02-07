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

#include "peakgraphics/core/GraphicsEntityComponent.hpp"

namespace peak
{
	namespace graphics
	{
		GraphicsEntityComponent::GraphicsEntityComponent(Entity *entity,
			Graphics *graphics) : EntityComponent(entity), graphics(graphics)
		{
		}
		GraphicsEntityComponent::~GraphicsEntityComponent()
		{
			std::map<std::string, SceneNode*>::iterator it = scenenodes.begin();
			while (it != scenenodes.end())
			{
				it->second->drop();
				it++;
			}
		}

		void GraphicsEntityComponent::addSceneNode(std::string name, SceneNode *node)
		{
			// Drop previous scene node with the same name
			SceneNode *prev = getSceneNode(name);
			if (prev)
			{
				prev->drop();
			}
			// Add new scene node
			node->grab();
			scenenodes[name] = node;
		}
		SceneNode *GraphicsEntityComponent::getSceneNode(std::string name)
		{
			std::map<std::string, SceneNode*>::iterator it = scenenodes.find(name);
			if (it == scenenodes.end())
				return 0;
			return it->second;
		}

		void GraphicsEntityComponent::update()
		{
			// Adjust position based on "position" property
			// TODO
		}
	}
}
