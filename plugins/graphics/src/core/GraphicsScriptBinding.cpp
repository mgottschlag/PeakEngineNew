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
#include "peakgraphics/scene/ModelSceneNode.hpp"
#include "peakgraphics/scene/LightSceneNode.hpp"
#include "peakgraphics/scene/GUISceneNode.hpp"
#include "peakgraphics/gui/ButtonElement.hpp"
#include "peakgraphics/gui/FrameElement.hpp"
#include "peakgraphics/gui/ListBoxElement.hpp"
#include "peakengine/core/Engine.hpp"

#include <luabind/operator.hpp>
#include <luabind/adopt_policy.hpp>

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
						.def("getEngine", &Graphics::getEngine)
						.def("getMouseMoveEvent", &Graphics::getMouseMoveEvent)
						.def("getMousePositionEvent", &Graphics::getMousePositionEvent)
						.def("getMouseButtonEvent", &Graphics::getMouseButtonEvent)
						.def("getKeyboardEvent", &Graphics::getKeyboardEvent)
						.def("getCharEvent", &Graphics::getCharEvent),
					// GraphicsEntityComponent
					luabind::class_<GraphicsEntityComponent, EntityComponent>("GraphicsEntityComponent")
						.def("addSceneNode", &GraphicsEntityComponent::addSceneNode, luabind::adopt(_3))
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
						.def("getParent", &SceneNode::getParent),
					// CameraSceneNode
					luabind::class_<CameraSceneNode, SceneNode>("CameraSceneNode"),
					// ModelSceneNode
					luabind::class_<ModelSceneNode, SceneNode>("ModelSceneNode"),
					// LightSceneNode
					luabind::class_<LightSceneNode, SceneNode>("LightSceneNode"),
					// GUISceneNode
					luabind::class_<GUISceneNode, SceneNode>("GUISceneNode")
						.def("injectMousePosition", &GUISceneNode::injectMousePosition)
						.def("injectMouseButton", &GUISceneNode::injectMouseButton)
						.def("injectChar", &GUISceneNode::injectChar)
						.def("injectKeyboard", &GUISceneNode::injectKeyboard)
						.def("getActionEvent", &GUISceneNode::getActionEvent)
						.def("getRootElement", &GUISceneNode::getRootElement),
					// GUIElement
					luabind::class_<GUIElement, Loadable, SharedPointer<GUIElement> >("GUIElement")
						.def("setPosition", &GUIElement::setPosition)
						.def("getPosition", &GUIElement::getPosition)
						.def("setSize", &GUIElement::setSize)
						.def("getSize", &GUIElement::getSize)
						.def("setVisible", &GUIElement::setVisible)
						.def("isVisible", &GUIElement::isVisible)
						.def("setBackground", &GUIElement::setBackground)
						.def("setActionID", &GUIElement::setActionID)
						.def("getActionID", &GUIElement::getActionID),
					// ButtonElement
					luabind::class_<ButtonElement, GUIElement, SharedPointer<ButtonElement> >("ButtonElement")
						.def(luabind::constructor<GUISceneNode*, GUIElement*, std::string>())
						.def("setLabel", (void (ButtonElement::*)(const std::string&))&ButtonElement::setLabel)
						.def("getLabel", &ButtonElement::getLabel),
					// FrameElement
					luabind::class_<FrameElement, GUIElement, SharedPointer<FrameElement> >("FrameElement")
						.def(luabind::constructor<GUISceneNode*, GUIElement*, std::string>()),
					// ListBoxElement
					luabind::class_<ListBoxElement, GUIElement, SharedPointer<ListBoxElement> >("ListBoxElement")
						.def(luabind::constructor<GUISceneNode*, GUIElement*, std::string>())
						.def("setTitle", &ListBoxElement::setTitle)
						.def("getTitle", &ListBoxElement::getTitle)
						.def("addRow", &ListBoxElement::addRow)
						.def("getRow", &ListBoxElement::getRow)
						.def("setRow", &ListBoxElement::setRow)
						.def("deleteRow", &ListBoxElement::deleteRow)
						.def("getRowCount", &ListBoxElement::getRowCount)
						.def("clearRows", &ListBoxElement::clearRows)
						.def("getSelection", &ListBoxElement::getSelection)
						.def("setSelection", &ListBoxElement::setSelection)
				]
			];
		}
	}
}
