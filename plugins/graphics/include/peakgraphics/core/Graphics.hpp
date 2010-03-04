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

#ifndef _PEAKGRAPHICS_CORE_GRAPHICS_HPP_
#define _PEAKGRAPHICS_CORE_GRAPHICS_HPP_

#include "../scene/SceneNode.hpp"
#include "peakengine/support/Event.hpp"
#include "KeyCode.hpp"

#include <queue>

namespace peak
{
	class Engine;
	class Game;

	namespace graphics
	{
		class CameraSceneNode;

		class Graphics
		{
			public:
				Graphics();
				~Graphics();

				bool init(int width, int height);
				bool shutdown();
				void resize(int width, int height);
				void registerComponents(Game *game);

				bool loadAll();

				void registerLoading(Loadable *loadable);
				void registerParentChange(SceneNode *node);

				SceneNode *getRootSceneNode()
				{
					return root;
				}

				void setDefaultCamera(CameraSceneNode *camera);

				void setEngine(Engine *engine)
				{
					this->engine = engine;
				}
				Engine *getEngine()
				{
					return engine;
				}

				Event2<int, int> &getMouseMoveEvent()
				{
					return mouseevent;
				}
				Event2<int, int> &getMousePositionEvent()
				{
					return mousepositionevent;
				}
				Event2<int, int> &getMouseButtonEvent()
				{
					return mousebuttonevent;
				}
				Event2<int, int> &getKeyboardEvent()
				{
					return keyboardevent;
				}
				Event1<int> &getCharEvent()
				{
					return charevent;
				}

				void injectMouseMovement(int x, int y, int dx, int dy);
				void injectMouseButton(int button, int state)
				{
					mousebuttonevent.trigger(button, state);
				}
				void injectKeyboard(KeyCode key, int state)
				{
					keyboardevent.trigger(key, state);
				}
				void injectChar(wchar_t c)
				{
					charevent.trigger(c);
				}

				bool render();
			private:
				Engine *engine;

				SceneNode *root;

				Mutex parentmutex;
				std::queue<SharedPointer<SceneNode> > parentchange;
				Mutex loadingmutex;
				std::queue<SharedPointer<Loadable> > loading;

				Mutex mutex;
				SharedPointer<CameraSceneNode> defcamera;

				Event2<int, int> mouseevent;
				Event2<int, int> mousepositionevent;
				Event2<int, int> mousebuttonevent;
				Event2<int, int> keyboardevent;
				Event1<int> charevent;
		};
	}
}

#endif
