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

#ifndef _PEAKGRAPHICS_CORE_GRAPHICSENTITYCOMPONENTFACTORY_HPP_
#define _PEAKGRAPHICS_CORE_GRAPHICSENTITYCOMPONENTFACTORY_HPP_

#include "peakengine/core/EntityComponentFactory.hpp"
#include "peakengine/support/Vector3.hpp"
#include "peakengine/support/Vector2.hpp"

#include <vector>

namespace peak
{
	namespace graphics
	{
		class Graphics;
		class SceneNode;

		class GraphicsEntityComponentTemplate : public EntityComponentTemplate
		{
			public:
				GraphicsEntityComponentTemplate(EntityComponentFactory *factory)
					: EntityComponentTemplate(factory)
				{
				}

				struct SceneNodeInfo
				{
					std::string name;
					Vector3F position;
					Vector3F rotation;
					Vector3F scale;
					bool visible;
				};

				struct ModelInfo
				{
					SceneNodeInfo info;
					std::string file;
				};
				std::vector<ModelInfo> models;

				struct LightInfo
				{
					SceneNodeInfo info;
					std::string file;
					std::string lighting;
					std::string shadow;
				};
				std::vector<LightInfo> lights;

				struct CameraInfo
				{
					SceneNodeInfo info;
					std::string file;
					bool defaultcam;
				};
				std::vector<CameraInfo> cameras;

				struct GUIInfo
				{
					SceneNodeInfo info;
					std::string file;
					Vector2I screensize;
				};
				std::vector<GUIInfo> guis;
		};

		class GraphicsEntityComponentFactory : public EntityComponentFactory
		{
			public:
				GraphicsEntityComponentFactory(Graphics *graphics)
					: EntityComponentFactory("Graphics"), graphics(graphics)
				{
				}

				virtual EntityComponentTemplate *createTemplate(TiXmlElement *xml);

				virtual EntityComponent *createComponent(Entity *entity);
				virtual EntityComponent *createComponent(Entity *entity,
					EntityComponentTemplate *tpl);
			private:
				bool readSceneNodeInfo(TiXmlElement *xml,
					GraphicsEntityComponentTemplate::SceneNodeInfo &info);
				void applySceneNodeInfo(SceneNode *node,
					const GraphicsEntityComponentTemplate::SceneNodeInfo &info);

				Graphics *graphics;
		};
	}
}

#endif
