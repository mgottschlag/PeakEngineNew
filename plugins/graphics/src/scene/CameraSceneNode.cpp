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

#include "peakgraphics/scene/CameraSceneNode.hpp"
#include "peakgraphics/core/Graphics.hpp"
#include "peakengine/core/Engine.hpp"

#include <Horde3D.h>

#include <iostream>

namespace peak
{
	namespace graphics
	{
		CameraSceneNode::CameraSceneNode(Graphics *graphics, std::string pipeline)
			: SceneNode(graphics), pipeline(pipeline)
		{
			graphics->registerLoading(this);
		}
		CameraSceneNode::~CameraSceneNode()
		{
		}

		bool CameraSceneNode::load()
		{
			mutex.lock();
			H3DRes piperes = h3dAddResource(H3DResTypes::Pipeline, pipeline.c_str(), 0);
			graphics->loadAll();
			node = h3dAddCameraNode(H3DRootNode, "Camera", piperes);
			std::cout << "Loaded camera." << std::endl;
			mutex.unlock();
			return true;
		}
	}
}
