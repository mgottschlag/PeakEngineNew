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

#ifndef _PEAKGRAPHICS_GUI_GUISCENENODE_HPP_
#define _PEAKGRAPHICS_GUI_GUISCENENODE_HPP_

#include "SceneNode.hpp"
#include "peakengine/support/Vector2.hpp"
#include "peakengine/support/Event.hpp"
#include "../core/KeyCode.hpp"

#include <string>
#include <queue>

namespace peak
{
	namespace graphics
	{
		class GUIElement;
		class InputEvent;

		class GUISceneNode : public SceneNode
		{
			public:
				GUISceneNode(Graphics *graphics, std::string skin,
					std::string layout = "");
				~GUISceneNode();

				virtual bool load();

				virtual void setScreenSize(Vector2I size);
				Vector2I getScreenSize()
				{
					return screensize;
				}

				void injectMousePosition(int x, int y);
				void injectMouseButton(unsigned int button, int state);
				void injectChar(int c);
				void injectKeyboard(KeyCode key, int state);

				void registerParentChange(GUIElement *element);

				virtual void update();

				Event1<int> &getActionEvent()
				{
					return actionevent;
				}
				GUIElement *getRootElement()
				{
					return root;
				}
			private:
				std::string skin;
				std::string layout;

				GUIElement *root;
				std::queue<GUIElement*> parentchange;

				Vector2I screensize;

				Event1<int> actionevent;

				std::queue<InputEvent*> input;
		};
	}
}

#endif
