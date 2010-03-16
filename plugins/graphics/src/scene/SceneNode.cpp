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

#include "peakgraphics/scene/SceneNode.hpp"
#include "peakgraphics/core/Graphics.hpp"

#include "Horde3D.h"

namespace peak
{
	namespace graphics
	{
		SceneNode::SceneNode(Graphics *graphics)
			: Loadable(), graphics(graphics), changed(false),
			scale(1.0f, 1.0f, 1.0f), visible(true), parent(0), newparent(0),
			node(0)
		{
		}
		SceneNode::~SceneNode()
		{
			// Delete children
			children.clear();
			// Remove scene node
			if (node)
			{
				h3dRemoveNode(node);
			}
		}

		void SceneNode::remove()
		{
			grab();
			mutex.lock();
			// Remove from parent
			if (newparent)
				newparent = 0;
			if (parent)
			{
				mutex.unlock();
				parent->mutex.lock();
				for (unsigned int i = 0; i < parent->children.size(); i++)
				{
					if (parent->children[i] == this)
					{
						parent->children.erase(parent->children.begin() + i);
						break;
					}
				}
				parent->mutex.unlock();
				mutex.lock();
			}
			mutex.unlock();
			drop();
		}

		void SceneNode::setPosition(Vector3F pos)
		{
			mutex.lock();
			position = pos;
			changed = true;
			mutex.unlock();
		}

		void SceneNode::setRotation(Quaternion rot)
		{
			mutex.lock();
			rotation = rot;
			changed = true;
			mutex.unlock();
		}

		void SceneNode::setScale(Vector3F scale)
		{
			mutex.lock();
			this->scale = scale;
			changed = true;
			mutex.unlock();
		}

		void SceneNode::setVisible(bool visible)
		{
			mutex.lock();
			if (this->visible != visible)
			{
				this->visible = visible;
				changed = true;
			}
			mutex.unlock();
		}

		void SceneNode::setParent(SceneNode *parent)
		{
			mutex.lock();
			if (parent != this->parent && parent != newparent)
			{
				// Register for parent change
				if (!newparent)
					graphics->registerParentChange(this);
				newparent = parent;
			}
			mutex.unlock();
		}
		SceneNode *SceneNode::getParent()
		{
			mutex.lock();
			SceneNode *parent = this->parent;
			if (newparent)
				parent = newparent;
			mutex.unlock();
			return parent;
		}

		void SceneNode::updateParent()
		{
			mutex.lock();
			if (!newparent)
			{
				mutex.unlock();
				return;
			}
			// We have to make sure the scene node does not get deleted
			grab();
			// Remove the node from the parent node
			if (parent)
				parent->removeChild(this);
			// Add to the new parent
			parent = newparent;
			parent->children.push_back(this);
			// Set parent
			h3dSetNodeParent(node, parent->node);
			newparent = 0;
			drop();
			mutex.unlock();
		}
		void SceneNode::update()
		{
			// Update position/rotation
			mutex.lock();
			if (changed && isLoaded())
			{
				Matrix4 rot = rotation.toMatrix();
				Matrix4 m = Matrix4::TransMat(position) * Matrix4::ScaleMat(scale) * rot;
				h3dSetNodeTransMat(node, m.x);
				h3dSetNodeActivation(node, visible);
				changed = false;
			}
			// Update children
			for (unsigned int i = 0; i < children.size(); i++)
			{
				mutex.unlock();
				children[i]->update();
				mutex.lock();
			}
			mutex.unlock();
		}

		void SceneNode::removeChild(SceneNode *child)
		{
			for (unsigned int i = 0; i < children.size(); i++)
			{
				if (children[i].get() == child)
				{
					children.erase(children.begin() + i);
					return;
				}
			}
		}
	}
}
