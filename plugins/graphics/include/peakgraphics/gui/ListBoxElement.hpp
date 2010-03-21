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

#ifndef _PEAKGRAPHICS_GUI_LISTBOXELEMENT_HPP_
#define _PEAKGRAPHICS_GUI_LISTBOXELEMENT_HPP_

#include "GUIElement.hpp"
#include "peakengine/support/Event.hpp"

namespace peak
{
	namespace graphics
	{
		class ListBoxElement : public GUIElement
		{
			public:
				ListBoxElement(GUISceneNode *node, GUIElement *parent = 0,
					const std::string &title = "");
				~ListBoxElement();

				virtual bool load();

				void setTitle(const std::string &title);
				std::string getTitle()
				{
					return title;
				}
				void addRow(const std::string &row);
				void setRow(unsigned int index, const std::string &row);
				std::string getRow(unsigned int index);
				void deleteRow(unsigned int index);
				unsigned int getRowCount();
				void clearRows();

				void setSelection(int selection);
				int getSelection()
				{
					return selection;
				}

				virtual void update();
			private:
				void updateData();
				std::wstring toWString(const std::string &s);

				std::string title;
				std::vector<std::string> rows;
				int selection;
		};
	}
}

#endif
