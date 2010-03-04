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

#include "peakgraphics/gui/GUIElement.hpp"
#include "peakgraphics/scene/GUISceneNode.hpp"


#include <Horde3DGUI.h>
#include <iostream>

namespace peak
{
	namespace graphics
	{
		GUIElement::GUIElement(GUISceneNode *node, GUIElement *parent)
			: node(node), gui(0), newparent(parent), parent(0), element(0),
			visible(true), background(false), actionid(0), changed(false)
		{
			position = ScreenPosition(Vector2F(0.0, 0.0), Vector2I(0, 0));
			size = ScreenPosition(Vector2F(1.0, 1.0), Vector2I(0, 0));
		}
		GUIElement::~GUIElement()
		{
		}

		bool GUIElement::load()
		{
			return false;
		}
		bool GUIElement::destroy()
		{
			// Remove children
			// TODO: Locking
			for (unsigned int i = 0; i < children.size(); i++)
			{
				children[i]->destroy();
				children[i]->setLoaded(false);
			}
			children.clear();
			// Remove from parent
			mutex.lock();
			if (parent)
				parent->removeChild(this);
			// Delete element
			if (element != 0)
			{
				h3dguiRemoveElement(node->getNode(), element);
				element = 0;
			}
			mutex.unlock();
			return true;
		}

		void GUIElement::setPosition(ScreenPosition position)
		{
			this->position = position;
			changed = true;
		}
		ScreenPosition GUIElement::getPosition()
		{
			return position;
		}

		void GUIElement::setSize(ScreenSize size)
		{
			this->size = size;
			changed = true;
		}
		ScreenSize GUIElement::getSize()
		{
			return size;
		}

		void GUIElement::setVisible(bool visible)
		{
			this->visible = visible;
			changed = true;
		}
		bool GUIElement::isVisible()
		{
			return visible;
		}

		void GUIElement::setBackground(bool background)
		{
			this->background = background;
			changed = true;
		}

		void GUIElement::setActionID(int actionid)
		{
			this->actionid = actionid;
			changed = true;
		}

		void GUIElement::updateParent()
		{
			mutex.lock();
			// We have to make sure the widget does not get deleted
			grab();
			// Remove the node from the parent node
			if (parent)
				parent->removeChild(this);
			// Add to the new parent
			parent = newparent;
			if (!parent)
				parent = node->getRootElement();
			parent->children.push_back(this);
			if (element && parent->element)
				h3dguiSetParent(node->getNode(), element, parent->element);
			newparent = 0;
			drop();
			mutex.unlock();
		}
		void GUIElement::updatePosition()
		{
			if (!element)
				return;
			// Set size/position
			h3dguiSetPosition(gui, element, position.rel.x, position.abs.x,
				position.rel.y, position.abs.y);
			h3dguiSetSize(gui, element, size.rel.x, size.abs.x, size.rel.y,
				size.abs.y);
		}
		void GUIElement::update()
		{
			if (element && changed)
			{
				// Apply position, size
				updatePosition();
				// Update visibility
				h3dguiSetVisible(node->getNode(), element, visible ? 1 : 0);
				// Update background
				h3dguiSetElementParamI(node->getNode(), element, H3DElementParam::ShowBackgroundI, background);
				// Update action ID
				h3dguiSetElementParamI(node->getNode(), element, H3DElementParam::ActionIdI, actionid);
				// Reset change flag
				changed = false;
			}
			// Update children
			for (unsigned int i = 0; i < children.size(); i++)
			{
				children[i]->update();
			}
		}

		void GUIElement::removeChild(GUIElement *child)
		{
			mutex.lock();
			for (unsigned int i = 0; i < children.size(); i++)
			{
				if (children[i].get() == child)
				{
					children.erase(children.begin() + i);
					return;
				}
			}
			mutex.unlock();
		}

		void GUIElement::initialUpdate()
		{
			gui = node->getNode();
			changed = true;
			updateParent();
			update();
		}
	}
}
