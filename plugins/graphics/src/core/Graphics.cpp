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

#include "peakgraphics/core/Graphics.hpp"
#include "peakgraphics/core/GraphicsWorldComponentFactory.hpp"
#include "peakgraphics/core/GraphicsEntityComponentFactory.hpp"
#include "peakgraphics/core/GraphicsScriptBinding.hpp"
#include "peakgraphics/scene/RootSceneNode.hpp"
#include "peakgraphics/scene/CameraSceneNode.hpp"
#include "peakengine/core/Engine.hpp"
#include "peakengine/core/Game.hpp"

#include <Horde3D.h>
#include <Horde3DUtils.h>

#include <string>

namespace peak
{
	namespace graphics
	{
		Graphics::Graphics() : engine(0)
		{
		}
		Graphics::~Graphics()
		{
		}

		bool Graphics::init(int width, int height)
		{
			if (!h3dInit())
			{
				h3dutDumpMessages();
				return false;
			}
			h3dSetOption(H3DOptions::LoadTextures, 1);
			h3dSetOption(H3DOptions::TexCompression, 0);
			h3dSetOption(H3DOptions::FastAnimation, 0);
			h3dSetOption(H3DOptions::MaxAnisotropy, 4);
			h3dSetOption(H3DOptions::ShadowMapSize, 2048);
			h3dSetupViewport(0, 0, width, height, true);
			root = new RootSceneNode(this);
			root->grab();
			return true;
		}
		bool Graphics::shutdown()
		{
			root->drop();
			h3dRelease();
			return true;
		}
		void Graphics::resize(int width, int height)
		{
			h3dSetupViewport(0, 0, width, height, true);
		}
		void Graphics::registerComponents(Game *game)
		{
			// Register world component factory
			GraphicsWorldComponentFactory *worldfactory;
			worldfactory = new GraphicsWorldComponentFactory(this);
			game->addWorldComponentFactory(worldfactory);
			// Register entity component factory
			GraphicsEntityComponentFactory *entityfactory;
			entityfactory = new GraphicsEntityComponentFactory(this);
			game->addEntityComponentFactory(entityfactory);
			// Register script bindings
			game->addScriptBinding(new GraphicsScriptBinding());
		}

		bool Graphics::loadAll()
		{
			std::string dir = engine->getDirectory() + "/Data/Graphics";
			bool success = h3dutLoadResourcesFromDisk(dir.c_str());
			h3dutDumpMessages();
			return success;
		}

		void Graphics::registerLoading(Loadable *loadable)
		{
			loadingmutex.lock();
			loading.push(loadable);
			loadingmutex.unlock();
		}
		void Graphics::registerParentChange(SceneNode *node)
		{
			parentmutex.lock();
			parentchange.push(node);
			parentmutex.unlock();
		}

		void Graphics::setDefaultCamera(CameraSceneNode *camera)
		{
			mutex.lock();
			defcamera = camera;
			mutex.unlock();
		}

		void Graphics::injectMouseMovement(int x, int y, int dx, int dy)
		{
			mousepositionevent.trigger(x, y);
			mouseevent.trigger(dx, dy);
		}

		bool Graphics::render()
		{
			loadingmutex.lock();
			// Load loadable objects
			while (loading.size() > 0)
			{
				SharedPointer<Loadable> loadable = loading.front();
				loading.pop();
				loadingmutex.unlock();
				loadable->tryLoading();
				loadingmutex.lock();
			}
			loadingmutex.unlock();
			// Update scene node tree
			parentmutex.lock();
			while (parentchange.size() > 0)
			{
				SharedPointer<SceneNode> node = parentchange.front();
				parentchange.pop();
				parentmutex.unlock();
				node->updateParent();
				parentmutex.lock();
			}
			parentmutex.unlock();
			// Update scene node positions
			root->update();
			// Render everything with the default camera
			mutex.lock();
			if (defcamera)
				h3dRender(defcamera->getNode());
			mutex.unlock();
			h3dFinalizeFrame();
			h3dutDumpMessages();
			return true;
		}
	}
}
