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

#ifndef _PEAKGRAPHICS_SCENE_SCENENODE_HPP_
#define _PEAKGRAPHICS_SCENE_SCENENODE_HPP_

#include "peakengine/support/Loadable.hpp"
#include "peakengine/support/Vector3.hpp"
#include "peakengine/support/Quaternion.hpp"
#include "peakengine/support/Mutex.hpp"

#include <vector>

namespace peak
{
	namespace graphics
	{
		class Graphics;

		class SceneNode : public Loadable
		{
			public:
				SceneNode(Graphics *graphics);
				virtual ~SceneNode();

				void setPosition(Vector3F pos);
				Vector3F getPosition()
				{
					return position;
				}

				void setRotation(Quaternion rot);
				Quaternion getRotation()
				{
					return rotation;
				}

				void setScale(Vector3F scale);
				Vector3F getScale()
				{
					return scale;
				}

				void setVisible(bool visible);
				bool isVisible()
				{
					return visible;
				}

				void setParent(SceneNode *parent);
				SceneNode *getParent();

				void updateParent();
				virtual void update();

				unsigned int getNode()
				{
					return node;
				}
				Graphics *getGraphics()
				{
					return graphics;
				}
			protected:
				void removeChild(SceneNode *child);

				Mutex mutex;

				Graphics *graphics;

				bool changed;
				Vector3F position;
				Quaternion rotation;
				Vector3F scale;
				bool visible;
				
				SceneNode *parent;
				SceneNode *newparent;
				std::vector<SharedPointer<SceneNode> > children;

				unsigned int node;
		};
	}
}

#endif
