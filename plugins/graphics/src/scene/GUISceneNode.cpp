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

#include "peakgraphics/scene/GUISceneNode.hpp"
#include "peakgraphics/core/Graphics.hpp"
#include "peakgraphics/gui/RootElement.hpp"

#include <Horde3D.h>
#include <Horde3DGUI.h>

#include <iostream>

namespace peak
{
	namespace graphics
	{
		struct InputEvent
		{
			virtual void apply(unsigned int node) = 0;
		};
		struct CharEvent : public InputEvent
		{
			CharEvent(wchar_t c) : c(c)
			{
			}
			virtual void apply(unsigned int node)
			{
				h3dguiInsertChar(node, c);
			}
			wchar_t c;
		};
		struct MousePosEvent : public InputEvent
		{
			MousePosEvent(float x, float y) : x(x), y(y)
			{
			}
			virtual void apply(unsigned int node)
			{
				h3dguiSetMousePosition(node, x, y);
			}
			float x;
			float y;
		};
		struct MouseButtonEvent : public InputEvent
		{
			MouseButtonEvent(int button, int state)
				: button(button), state(state)
			{
			}
			virtual void apply(unsigned int node)
			{
				h3dguiSetMouseButton(node, button, state);
			}
			int button;
			int state;
		};

		GUISceneNode::GUISceneNode(Graphics *graphics, std::string skin,
			std::string layout)
			: SceneNode(graphics), skin(skin), layout(layout), root(0),
			screensize(640, 480)
		{
			graphics->registerLoading(this);
			root = new RootElement(this);
			// TODO: Release the root again
			root->grab();
			graphics->registerLoading(root);
		}
		GUISceneNode::~GUISceneNode()
		{
			while (!input.empty())
			{
				delete input.front();
				input.pop();
			}
		}

		bool GUISceneNode::load()
		{
			mutex.lock();
			H3DRes skinres = h3dAddResource(H3DGUI_ResType_Skin, skin.c_str(), 0);
			H3DRes layoutres = 0;
			#if 0
			if (layout != "")
				layoutres = h3dAddResource(H3DGUI_ResType_Layout, skin.c_str(), 0);
			#endif
			graphics->loadAll();
			node = h3dguiAddGUI(H3DRootNode, "GUI", skinres);
			#if 0
			// Apply layout
			// TODO
			#endif
			mutex.unlock();
			return true;
		}

		void GUISceneNode::setScreenSize(Vector2I size)
		{
			mutex.lock();
			screensize = size;
			changed = true;
			mutex.unlock();
		}

		void GUISceneNode::injectMousePosition(int x, int y)
		{
			mutex.lock();
			float fx = (float)x / screensize.x;
			float fy = (float)y / screensize.y;
			input.push(new MousePosEvent(fx, fy));
			mutex.unlock();
		}
		void GUISceneNode::injectMouseButton(unsigned int button, int state)
		{
			mutex.lock();
			input.push(new MouseButtonEvent(button, state));
			mutex.unlock();
		}
		void GUISceneNode::injectChar(int c)
		{
			mutex.lock();
			input.push(new CharEvent(c));
			mutex.unlock();
		}
		void GUISceneNode::injectKeyboard(KeyCode key, int state)
		{
		}

		void GUISceneNode::registerParentChange(GUIElement *element)
		{
			mutex.lock();
			element->grab();
			parentchange.push(element);
			mutex.unlock();
		}
		void GUISceneNode::update()
		{
			mutex.lock();
			if (isLoaded())
			{
				// Insert input
				while (!input.empty())
				{
					input.front()->apply(node);
					delete input.front();
					input.pop();
				}
				// Update GUI elements
				while (parentchange.size() > 0)
				{
					GUIElement *element = parentchange.front();
					parentchange.pop();
					element->updateParent();
					element->drop();
				}
				root->update();
				// Receive events
				while(h3dguiHasEvent(node))
				{
					int type = h3dguiGetEventType(node);
					unsigned int actionid = h3dguiGetEventActionID(node);
					if (type == H3DGUIEvent::Action)
					{
						actionevent.trigger(actionid);
					}
					h3dguiDeleteEvent(node);
				}
			}
			if (changed && isLoaded())
			{
				// Change virtual screen size
				h3dSetNodeParamI(node, H3DGUINode::WidthI, screensize.x);
				h3dSetNodeParamI(node, H3DGUINode::HeightI, screensize.y);
			}
			mutex.unlock();
			SceneNode::update();
		}
	}
}
