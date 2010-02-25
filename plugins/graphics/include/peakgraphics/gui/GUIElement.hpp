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

#ifndef _PEAKGRAPHICS_GUI_GUIELEMENT_HPP_
#define _PEAKGRAPHICS_GUI_GUIELEMENT_HPP_

#include "peakengine/support/ScreenPosition.hpp"
#include "peakengine/support/Mutex.hpp"
#include "peakengine/support/Loadable.hpp"

#include <vector>

namespace peak
{
	namespace graphics
	{
		class GUISceneNode;

		class GUIElement : public Loadable
		{
			public:
				GUIElement(GUISceneNode *node, GUIElement *parent = 0);
				~GUIElement();

				virtual bool load();
				virtual bool destroy();

				void setPosition(ScreenPosition position);
				ScreenPosition getPosition();

				void setSize(ScreenSize size);
				ScreenSize getSize();

				virtual void setVisible(bool visible);
				virtual bool isVisible();

				void setActionID(int actionid);
				int getActionID()
				{
					return actionid;
				}

				virtual void updateParent();
				virtual void updatePosition();
				virtual void update();

				unsigned int getElement()
				{
					return element;
				}
			protected:
				void removeChild(GUIElement *child);
				void initialUpdate();

				GUISceneNode *node;
				unsigned int gui;
				GUIElement *newparent;
				GUIElement *parent;
				std::vector<SharedPointer<GUIElement> > children;

				unsigned int element;

				ScreenPosition position;
				ScreenPosition size;
				bool visible;
				int actionid;

				bool changed;

				Mutex mutex;
		};
	}
}

#endif
