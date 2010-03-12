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

#include "peakphysics/core/PhysicsEntityComponentFactory.hpp"
#include "peakphysics/core/PhysicsEntityComponent.hpp"
#include "peakphysics/core/PhysicsWorldComponent.hpp"
#include "peakphysics/physics/Box.hpp"
#include "peakphysics/physics/Plane.hpp"
#include "peakphysics/physics/Body.hpp"
#include "peakphysics/physics/CharacterController.hpp"
#include "peakengine/core/World.hpp"
#include "peakengine/core/Entity.hpp"

#include "peakengine/support/tinyxml.h"

#include <iostream>

namespace peak
{
	namespace physics
	{
		Shape *PhysicsEntityComponentTemplate::BoxInfo::create()
		{
			Box *box = new Box;
			if (!box->init(size, mass))
			{
				delete box;
				return 0;
			}
			return box;
		}
		Shape *PhysicsEntityComponentTemplate::PlaneInfo::create()
		{
			Plane *plane = new Plane;
			if (!plane->init(normal, 0))
			{
				delete plane;
				return 0;
			}
			return plane;
		}

		EntityComponentTemplate *PhysicsEntityComponentFactory::createTemplate(TiXmlElement *xml)
		{
			// Create template
			PhysicsEntityComponentTemplate *tpl;
			tpl = new PhysicsEntityComponentTemplate(this);
			// Parse XML data
			// Bodies
			TiXmlNode *bodynode = xml->FirstChild("Body");
			while (bodynode)
			{
				TiXmlElement *bodyelem = bodynode->ToElement();
				if (!bodyelem)
				{
					bodynode = xml->IterateChildren("Body", bodynode);
					continue;
				}
				// Get body information
				PhysicsEntityComponentTemplate::BodyInfo info;
				if (parseBody(info, bodyelem))
				{
					tpl->bodies.push_back(info);
				}
				bodynode = xml->IterateChildren("Body", bodynode);
			}
			// Character controllers
			for (TiXmlNode *charnode = xml->FirstChild("CharacterController");
				charnode != 0;
				charnode = xml->IterateChildren("CharacterController", charnode))
			{
				TiXmlElement *charelem = charnode->ToElement();
				if (!charelem)
					continue;
				// Get character controller information
				PhysicsEntityComponentTemplate::CharacterInfo info;
				if (!charelem->Attribute("name"))
					continue;
				info.name = charelem->Attribute("name");
				tpl->charcontrollers.push_back(info);
			}
			return tpl;
		}

		EntityComponent *PhysicsEntityComponentFactory::createComponent(Entity *entity)
		{
			// Get world component (holds the simulation)
			World *world = entity->getWorld();
			PhysicsWorldComponent *worldcomponent;
			worldcomponent = (PhysicsWorldComponent*)world->getComponent(EWCT_Physics);
			if (!worldcomponent)
				return 0;
			// Create entity component
			PhysicsEntityComponent *component = new PhysicsEntityComponent(entity,
				worldcomponent->getSimulation());
			return component;
		}
		EntityComponent *PhysicsEntityComponentFactory::createComponent(Entity *entity,
			EntityComponentTemplate *tpl)
		{
			PhysicsEntityComponentTemplate *ptpl = (PhysicsEntityComponentTemplate*)tpl;
			// Get world component (holds the simulation)
			World *world = entity->getWorld();
			PhysicsWorldComponent *worldcomponent;
			worldcomponent = (PhysicsWorldComponent*)world->getComponent(EWCT_Physics);
			if (!worldcomponent)
				return 0;
			Simulation *simulation = worldcomponent->getSimulation();
			// Create entity component
			PhysicsEntityComponent *component = new PhysicsEntityComponent(entity,
				simulation);
			// Create bodies
			for (unsigned int i = 0; i < ptpl->bodies.size(); i++)
			{
				PhysicsEntityComponentTemplate::BodyInfo &bodyinfo = ptpl->bodies[i];
				// Create shapes
				std::vector<Shape*> shapes;
				for (unsigned int j = 0; j < bodyinfo.shapes.size(); j++)
				{
					std::cout << "Shape: " << bodyinfo.shapes[j] << std::endl;
					Shape *shape = bodyinfo.shapes[j]->create();
					if (shape)
						shapes.push_back(shape);
				}
				if (shapes.size() == 0 || shapes.size() > 1)
				{
					std::cout << "0 or too many shapes provided." << std::endl;
					for (unsigned int i = 0; i < shapes.size(); i++)
					{
						delete shapes[i];
					}
					continue;
				}
				// Create body
				Body *body = new Body();
				body->init(simulation, shapes[0]);
				body->setPosition(bodyinfo.position);
				body->setRotation(bodyinfo.rotation);
				component->addBody(bodyinfo.name, body);
			}
			// Create character controllers
			for (unsigned int i = 0; i < ptpl->charcontrollers.size(); i++)
			{
				PhysicsEntityComponentTemplate::CharacterInfo &charinfo = ptpl->charcontrollers[i];
				CharacterController *controller = new CharacterController();
				controller->init(simulation);
				component->addCharacterController(charinfo.name, controller);
			}
			return component;
		}

		bool PhysicsEntityComponentFactory::parseBody(PhysicsEntityComponentTemplate::BodyInfo &info,
			TiXmlElement *xml)
		{
			// Get body name
			if (!xml->Attribute("name"))
				return false;
			info.name = xml->Attribute("name");
			// Position/rotation
			info.position = Vector3F(0, 0, 0);
			if (xml->Attribute("position"))
				info.position = xml->Attribute("position");
			info.rotation = Vector3F(0, 0, 0);
			if (xml->Attribute("rotation"))
				info.rotation = xml->Attribute("rotation");
			// Boxes
			for (TiXmlNode *boxnode = xml->FirstChild("Box"); boxnode != 0;
				boxnode = xml->IterateChildren("Box", boxnode))
			{
				TiXmlElement *boxelem = boxnode->ToElement();
				if (!boxelem)
					continue;
				PhysicsEntityComponentTemplate::BoxInfo *box;
				box = new PhysicsEntityComponentTemplate::BoxInfo;
				box->size = Vector3F(1, 1, 1);
				box->mass = 0.0f;
				if (boxelem->Attribute("size"))
					box->size.set(boxelem->Attribute("size"));
				if (boxelem->Attribute("mass"))
					box->mass = atof(boxelem->Attribute("mass"));
				info.shapes.push_back(box);
			}
			// Planes
			for (TiXmlNode *planenode = xml->FirstChild("Plane"); planenode != 0;
				planenode = xml->IterateChildren("Plane", planenode))
			{
				TiXmlElement *planeelem = planenode->ToElement();
				if (!planeelem)
					continue;
				PhysicsEntityComponentTemplate::PlaneInfo *plane;
				plane = new PhysicsEntityComponentTemplate::PlaneInfo;
				plane->normal = Vector3F(0, 1, 0);
				plane->mass = 0.0f;
				if (planeelem->Attribute("normal"))
					plane->normal.set(planeelem->Attribute("normal"));
				if (planeelem->Attribute("mass"))
					plane->mass = atof(planeelem->Attribute("mass"));
				info.shapes.push_back(plane);
			}
			return true;
		}
	}
}
