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

#include "peakgraphics/gui/FrameElement.hpp"
#include "peakgraphics/scene/GUISceneNode.hpp"
#include "peakgraphics/core/Graphics.hpp"

#include <Horde3DGUI.h>
#include <cstdlib>
#include <iostream>

namespace peak
{
	namespace graphics
	{
		FrameElement::FrameElement(GUISceneNode *node, GUIElement *parent,
			const std::string &label)
			: GUIElement(node, parent), label(label)
		{
			node->getGraphics()->registerLoading(this);
		}
		FrameElement::~FrameElement()
		{
		}

		bool FrameElement::load()
		{
			mutex.lock();
			element = h3dguiAddFrame(node->getNode(),
				h3dguiGetRoot(node->getNode()));
			mutex.unlock();
			initialUpdate();
			return true;
		}

		void FrameElement::setLabel(const std::string &label)
		{
			mutex.lock();
			this->label = label;
			changed = true;
			mutex.unlock();
		}

		void FrameElement::update()
		{
			mutex.lock();
			if (changed && element)
			{
				setLabel();
			}
			mutex.unlock();
			GUIElement::update();
		}

		void FrameElement::setLabel()
		{
			wchar_t *wstr = new wchar_t[label.size() + 1];
			mbstowcs(wstr, label.c_str(), label.size() + 1);
			h3dguiSetElementParamStr(node->getNode(), element,
				FrameParam::LabelStr, wstr);
			delete[] wstr;
		}
	}
}
