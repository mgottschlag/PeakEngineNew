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

#include "peakgraphics/core/GraphicsEntityComponentFactory.hpp"
#include "peakgraphics/core/GraphicsEntityComponent.hpp"
#include "peakgraphics/core/Graphics.hpp"
#include "peakgraphics/scene/ModelSceneNode.hpp"
#include "peakgraphics/scene/CameraSceneNode.hpp"

#include "peakengine/support/tinyxml.h"

#include <iostream>

namespace peak
{
	namespace graphics
	{
		EntityComponentTemplate *GraphicsEntityComponentFactory::createTemplate(TiXmlElement *xml)
		{
			// Create template
			GraphicsEntityComponentTemplate *tpl;
			tpl = new GraphicsEntityComponentTemplate(this);
			// Parse XML data
			// Model scene nodes
			TiXmlNode *modelnode = xml->FirstChild("Model");
			while (modelnode)
			{
				TiXmlElement *modelelem = modelnode->ToElement();
				if (!modelelem)
				{
					modelnode = xml->IterateChildren("Model", modelnode);
					continue;
				}
				if (!modelelem->Attribute("file")
					|| !modelelem->Attribute("name"))
				{
					std::cout << "Model file or name missing." << std::endl;
					modelnode = xml->IterateChildren("Model", modelnode);
					continue;
				}
				// Get component factory information
				std::string file = modelelem->Attribute("file");
				std::string name = modelelem->Attribute("name");
				GraphicsEntityComponentTemplate::ModelInfo modelinfo;
				modelinfo.file = file;
				modelinfo.name = name;
				tpl->models.push_back(modelinfo);
				// Create model
				modelnode = xml->IterateChildren("Model", modelnode);
			}
			// Camera scene nodes
			// TODO
			return tpl;
		}

		EntityComponent *GraphicsEntityComponentFactory::createComponent(Entity *entity)
		{
			GraphicsEntityComponent *component = new GraphicsEntityComponent(entity,
				graphics);
			return component;
		}
		EntityComponent *GraphicsEntityComponentFactory::createComponent(Entity *entity,
			EntityComponentTemplate *tpl)
		{
			// Create component
			GraphicsEntityComponent *component = new GraphicsEntityComponent(entity,
				graphics);
			if (!tpl || tpl->getFactory() != this)
				return component;
			GraphicsEntityComponentTemplate *gtpl = (GraphicsEntityComponentTemplate*)tpl;
			// Add model scene nodes
			for (unsigned int i = 0; i < gtpl->models.size(); i++)
			{
				peak::graphics::ModelSceneNode *model = new peak::graphics::ModelSceneNode(graphics,
					gtpl->models[i].file.c_str());
				// TODO: Group scene node
				model->setParent(graphics->getRootSceneNode());
				component->addSceneNode(gtpl->models[i].name, model);
			}
			return component;
		}
	}
}
