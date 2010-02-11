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

#include "peakgraphics/core/GraphicsScriptBinding.hpp"
#include "peakgraphics/core/GraphicsEntityComponent.hpp"
#include "peakgraphics/core/Graphics.hpp"
#include "peakgraphics/scene/SceneNode.hpp"
#include "peakgraphics/scene/CameraSceneNode.hpp"
#include "peakengine/core/Engine.hpp"

#include <luabind/operator.hpp>

namespace peak
{
	namespace graphics
	{
		template<class T> static T *get_pointer(const SharedPointer<T> &p)
		{
			return p.get();
		}

		template<class A> static SharedPointer<A const> *get_const_holder(SharedPointer<A>*)
		{
			return 0;
		}

		void GraphicsScriptBinding::apply(Script *script)
		{
			lua_State *state = script->getState();
			luabind::module(state, "peak")
			[
				luabind::namespace_("graphics")
				[
					// Graphics
					luabind::class_<Graphics>("Graphics")
						.def("loadAll", &Graphics::loadAll)
						.def("getRootSceneNode", &Graphics::getRootSceneNode)
						.def("setDefaultCamera", &Graphics::setDefaultCamera)
						.def("setDefaultCamera", &Graphics::setDefaultCamera)
						.def("getEngine", &Graphics::getEngine)
						.def("getMouseMoveEvent", &Graphics::getMouseMoveEvent),
					// GraphicsEntityComponent
					luabind::class_<GraphicsEntityComponent, EntityComponent>("GraphicsEntityComponent")
						.def("addSceneNode", &GraphicsEntityComponent::addSceneNode)
						.def("getSceneNode", &GraphicsEntityComponent::getSceneNode)
						.def("getGraphics", &GraphicsEntityComponent::getGraphics),
					// SceneNode
					luabind::class_<SceneNode>("SceneNode")
						.def("setPosition", &SceneNode::setPosition)
						.def("getPosition", &SceneNode::getPosition)
						.def("setRotation", &SceneNode::setRotation)
						.def("getRotation", &SceneNode::getRotation)
						.def("setScale", &SceneNode::setScale)
						.def("getScale", &SceneNode::getScale)
						.def("setVisible", &SceneNode::setVisible)
						.def("isVisible", &SceneNode::isVisible)
						.def("setParent", &SceneNode::setParent)
						.def("getParent", &SceneNode::getParent)
				]
			];
		}
	}
}
