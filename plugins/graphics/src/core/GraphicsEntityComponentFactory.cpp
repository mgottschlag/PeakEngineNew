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
#include "peakgraphics/scene/LightSceneNode.hpp"
#include "peakgraphics/scene/GroupSceneNode.hpp"
#include "peakgraphics/scene/GUISceneNode.hpp"

#include "peakengine/support/tinyxml.h"

#include <iostream>
#include <sstream>

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
				GraphicsEntityComponentTemplate::ModelInfo modelinfo;
				if (!readSceneNodeInfo(modelelem, modelinfo.info)
					|| !modelelem->Attribute("file"))
				{
					std::cout << "Model file or name missing." << std::endl;
					modelnode = xml->IterateChildren("Model", modelnode);
					continue;
				}
				// Get component factory information
				std::string file = modelelem->Attribute("file");
				modelinfo.file = file;
				tpl->models.push_back(modelinfo);
				modelnode = xml->IterateChildren("Model", modelnode);
			}
			// Light scene nodes
			TiXmlNode *lightnode = xml->FirstChild("Light");
			while (lightnode)
			{
				TiXmlElement *lightelem = lightnode->ToElement();
				if (!lightelem)
				{
					lightnode = xml->IterateChildren("Light", lightnode);
					continue;
				}
				GraphicsEntityComponentTemplate::LightInfo lightinfo;
				if (!readSceneNodeInfo(lightelem, lightinfo.info)
					|| !lightelem->Attribute("file")
					|| !lightelem->Attribute("shadow")
					|| !lightelem->Attribute("lighting"))
				{
					std::cout << "Light file, contexts or name missing." << std::endl;
					lightnode = xml->IterateChildren("Light", lightnode);
					continue;
				}
				// Get component factory information
				std::string lighting = lightelem->Attribute("lighting");
				std::string shadow = lightelem->Attribute("shadow");
				std::string file = lightelem->Attribute("file");
				lightinfo.file = file;
				lightinfo.shadow = shadow;
				lightinfo.lighting = lighting;
				tpl->lights.push_back(lightinfo);
				// Create model
				lightnode = xml->IterateChildren("Light", lightnode);
			}
			// Camera scene nodes
			TiXmlNode *cameranode = xml->FirstChild("Camera");
			while (cameranode)
			{
				TiXmlElement *cameraelem = cameranode->ToElement();
				if (!cameraelem)
				{
					cameranode = xml->IterateChildren("Camera", cameranode);
					continue;
				}
				GraphicsEntityComponentTemplate::CameraInfo camerainfo;
				if (!readSceneNodeInfo(cameraelem, camerainfo.info)
					|| !cameraelem->Attribute("pipeline"))
				{
					std::cout << "Camera pipeline or name missing." << std::endl;
					cameranode = xml->IterateChildren("Camera", cameranode);
					continue;
				}
				camerainfo.defaultcam = false;
				if (cameraelem->Attribute("default"))
					camerainfo.defaultcam = !strcmp(cameraelem->Attribute("default"), "yes");
				// Get component factory information
				std::string file = cameraelem->Attribute("pipeline");
				camerainfo.file = file;
				tpl->cameras.push_back(camerainfo);
				// Create model
				cameranode = xml->IterateChildren("Camera", cameranode);
			}
			// GUI scene nodes
			TiXmlNode *guinode = xml->FirstChild("GUI");
			while (guinode)
			{
				TiXmlElement *guielem = guinode->ToElement();
				if (!guielem)
				{
					guinode = xml->IterateChildren("GUI", guinode);
					continue;
				}
				GraphicsEntityComponentTemplate::GUIInfo guiinfo;
				if (!readSceneNodeInfo(guielem, guiinfo.info)
					|| !guielem->Attribute("skin"))
				{
					std::cout << "GUI skin or name missing." << std::endl;
					guinode = xml->IterateChildren("GUI", guinode);
					continue;
				}
				// Get component factory information
				std::string file = guielem->Attribute("skin");
				guiinfo.file = file;
				guiinfo.screensize = Vector2I(640, 480);
				if (guielem->Attribute("screensize"))
					guiinfo.screensize = guielem->Attribute("screensize");
				tpl->guis.push_back(guiinfo);
				// Create model
				guinode = xml->IterateChildren("GUI", guinode);
			}
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
			// Add root scene node
			GroupSceneNode *root = new GroupSceneNode(graphics);
			root->setParent(graphics->getRootSceneNode());
			component->addSceneNode("", root);
			// Add model scene nodes
			for (unsigned int i = 0; i < gtpl->models.size(); i++)
			{
				GraphicsEntityComponentTemplate::ModelInfo &info = gtpl->models[i];
				peak::graphics::ModelSceneNode *model = new peak::graphics::ModelSceneNode(graphics,
					info.file);
				model->setParent(root);
				model->setPosition(info.info.position);
				model->setRotation(info.info.rotation);
				model->setScale(info.info.scale);
				component->addSceneNode(info.info.name, model);
			}
			// Add light scene nodes
			for (unsigned int i = 0; i < gtpl->lights.size(); i++)
			{
				GraphicsEntityComponentTemplate::LightInfo &info = gtpl->lights[i];
				peak::graphics::LightSceneNode *light = new peak::graphics::LightSceneNode(graphics,
					info.file, info.lighting, info.shadow);
				light->setParent(root);
				light->setPosition(info.info.position);
				light->setRotation(info.info.rotation);
				light->setScale(info.info.scale);
				component->addSceneNode(info.info.name, light);
			}
			// Add camera scene nodes
			for (unsigned int i = 0; i < gtpl->cameras.size(); i++)
			{
				GraphicsEntityComponentTemplate::CameraInfo &info = gtpl->cameras[i];
				peak::graphics::CameraSceneNode *camera = new peak::graphics::CameraSceneNode(graphics,
					info.file);
				camera->setParent(root);
				camera->setPosition(info.info.position);
				camera->setRotation(info.info.rotation);
				camera->setScale(info.info.scale);
				component->addSceneNode(info.info.name, camera);
				if (info.defaultcam)
					graphics->setDefaultCamera(camera);
			}
			// Add gui scene nodes
			for (unsigned int i = 0; i < gtpl->guis.size(); i++)
			{
				GraphicsEntityComponentTemplate::GUIInfo &info = gtpl->guis[i];
				peak::graphics::GUISceneNode *gui = new peak::graphics::GUISceneNode(graphics,
					info.file);
				gui->setParent(root);
				gui->setPosition(info.info.position);
				gui->setRotation(info.info.rotation);
				gui->setScale(info.info.scale);
				gui->setScreenSize(info.screensize);
				component->addSceneNode(info.info.name, gui);
			}
			return component;
		}

		bool GraphicsEntityComponentFactory::readSceneNodeInfo(TiXmlElement *xml,
			GraphicsEntityComponentTemplate::SceneNodeInfo &info)
		{
			// Read name (needed)
			if (!xml->Attribute("name"))
				return false;
			info.name = xml->Attribute("name");
			// Read position/rotation/scale (optional)
			if (xml->Attribute("position"))
			{
				std::istringstream in(xml->Attribute("position"));
				char sep;
				in >> info.position.x >> sep  >> info.position.y >> sep  >> info.position.z;
			}
			else
				info.position = Vector3F(0, 0, 0);
			if (xml->Attribute("scale"))
			{
				std::istringstream in(xml->Attribute("scale"));
				char sep;
				in >> info.scale.x >> sep  >> info.scale.y >> sep  >> info.scale.z;
			}
			else
				info.scale = Vector3F(1, 1, 1);
			if (xml->Attribute("rotation"))
			{
				std::istringstream in(xml->Attribute("rotation"));
				char sep;
				in >> info.rotation.x >> sep  >> info.rotation.y >> sep  >> info.rotation.z;
			}
			else
				info.rotation = Vector3F(0, 0, 0);
			return true;
		}
	}
}
