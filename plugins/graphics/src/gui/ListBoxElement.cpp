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

#include "peakgraphics/gui/ListBoxElement.hpp"
#include "peakgraphics/scene/GUISceneNode.hpp"
#include "peakgraphics/core/Graphics.hpp"

#include <Horde3DGUI.h>
#include <cstdlib>

namespace peak
{
	namespace graphics
	{
		ListBoxElement::ListBoxElement(GUISceneNode *node, GUIElement *parent,
			const std::string &title)
			: GUIElement(node, parent), title(title), selection(-1)
		{
			node->getGraphics()->registerLoading(this);
		}
		ListBoxElement::~ListBoxElement()
		{
		}

		bool ListBoxElement::load()
		{
			mutex.lock();
			element = h3dguiAddListBox(node->getNode(),
				h3dguiGetRoot(node->getNode()));
			mutex.unlock();
			initialUpdate();
			return true;
		}

		void ListBoxElement::setTitle(const std::string &title)
		{
			mutex.lock();
			this->title = title;
			changed = true;
			mutex.unlock();
		}

		void ListBoxElement::update()
		{
			mutex.lock();
			if (changed && element)
			{
				updateData();
			}
			else
			{
				selection = h3dguiGetElementParamI(node->getNode(), element,
					ListBoxParam::SelectionI);
			}
			mutex.unlock();
			GUIElement::update();
		}

		void ListBoxElement::addRow(const std::string &row)
		{
			mutex.lock();
			rows.push_back(row);
			changed = true;
			mutex.unlock();
		}
		void ListBoxElement::setRow(unsigned int index, const std::string &row)
		{
			mutex.lock();
			if (index < rows.size())
			{
				rows[index] = row;
				changed = true;
			}
			mutex.unlock();
		}
		std::string ListBoxElement::getRow(unsigned int index)
		{
			mutex.lock();
			std::string row;
			if (index < rows.size())
				row = rows[index];
			mutex.unlock();
			return row;
		}
		void ListBoxElement::deleteRow(unsigned int index)
		{
			mutex.lock();
			if (index < rows.size())
			{
				if (selection == (int)index)
					selection = -1;
				rows.erase(rows.begin() + index);
				changed = true;
			}
			mutex.unlock();
		}
		unsigned int ListBoxElement::getRowCount()
		{
			mutex.lock();
			unsigned int count = rows.size();
			mutex.unlock();
			return count;
		}
		void ListBoxElement::clearRows()
		{
			mutex.lock();
			selection = -1;
			rows.clear();
			changed = true;
			mutex.unlock();
		}

		void ListBoxElement::setSelection(int selection)
		{
			mutex.lock();
			this->selection = selection;
			changed = true;
			mutex.unlock();
		}

		void ListBoxElement::updateData()
		{
			// Update title
			h3dguiSetListBoxTitles(node->getNode(), element,
				toWString(title).c_str());
			// Delete rows
			h3dguiClearListBox(node->getNode(), element);
			// Readd rows
			for (unsigned int i = 0; i < rows.size(); i++)
				h3dguiAddListBoxRow(node->getNode(), element,
					toWString(rows[i]).c_str());
			// Set selection
			h3dguiSetElementParamI(node->getNode(), element,
					ListBoxParam::SelectionI, selection);
		}
		std::wstring ListBoxElement::toWString(const std::string &s)
		{
			wchar_t *wstr = new wchar_t[s.size() + 1];
			mbstowcs(wstr, s.c_str(), s.size() + 1);
			std::wstring w(wstr);
			delete[] wstr;
			return w;
		}
	}
}
