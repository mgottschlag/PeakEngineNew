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

#include "peakgraphics/scene/GroupSceneNode.hpp"
#include "peakgraphics/core/Graphics.hpp"

#include <Horde3D.h>

namespace peak
{
	namespace graphics
	{
		GroupSceneNode::GroupSceneNode(Graphics *graphics)
			: SceneNode(graphics)
		{
			graphics->registerLoading(this);
		}
		GroupSceneNode::~GroupSceneNode()
		{
		}

		bool GroupSceneNode::load()
		{
			mutex.lock();
			node = h3dAddGroupNode(H3DRootNode, "Group");
			mutex.unlock();
			return true;
		}
	}
}
