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

#ifndef _PEAKGRAPHICS_CORE_GRAPHICSENTITYCOMPONENT_HPP_
#define _PEAKGRAPHICS_CORE_GRAPHICSENTITYCOMPONENT_HPP_

#include "peakengine/core/EntityComponent.hpp"
#include "../scene/SceneNode.hpp"

#include <string>
#include <map>

namespace peak
{
	namespace graphics
	{
		class GraphicsEntityComponent : public EntityComponent
		{
			public:
				GraphicsEntityComponent(Entity *entity, Graphics *graphics);
				virtual ~GraphicsEntityComponent();

				void addSceneNode(std::string name, SceneNode *node);
				SceneNode *getSceneNode(std::string name);

				virtual int getType()
				{
					return EECT_Graphics;
				}

				virtual void update();

				Graphics *getGraphics()
				{
					return graphics;
				}
			private:
				Graphics *graphics;

				SceneNode *rootnode;

				std::map<std::string, SceneNode*> scenenodes;
		};
	}
}

#endif
