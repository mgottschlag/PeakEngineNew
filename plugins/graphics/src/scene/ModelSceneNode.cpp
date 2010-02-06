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

#include "peakgraphics/scene/ModelSceneNode.hpp"
#include "peakgraphics/core/Graphics.hpp"

#include <Horde3D.h>

#include <iostream>

namespace peak
{
	namespace graphics
	{
		ModelSceneNode::ModelSceneNode(Graphics *graphics, std::string model)
			: SceneNode(graphics), model(model)
		{
			graphics->registerLoading(this);
		}
		ModelSceneNode::~ModelSceneNode()
		{
		}

		bool ModelSceneNode::load()
		{
			mutex.lock();
			H3DRes modelres = h3dAddResource(H3DResTypes::SceneGraph, model.c_str(), 0);
			graphics->loadAll();
			node = h3dAddNodes(H3DRootNode, modelres);
			std::cout << "Loaded model." << std::endl;
			mutex.unlock();
			return true;
		}
	}
}
