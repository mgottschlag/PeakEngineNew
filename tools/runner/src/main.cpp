
#include <PeakEngine.hpp>
#include <PeakGraphics.hpp>
#include <PeakNetwork.hpp>
#include <PeakPhysics.hpp>

#include <iostream>
#include <GL/glfw.h>

peak::graphics::Graphics graphics;
peak::network::Network network;
peak::physics::Physics physics;

bool mouselocked = true;

static int oldx;
static int oldy;

peak::graphics::KeyCode convertKey(int key)
{
	switch (key)
	{
		case GLFW_KEY_SPACE:
			return peak::graphics::EK_Space;
		case GLFW_KEY_ESC:
			return peak::graphics::EK_Escape;
		case GLFW_KEY_F1:
			return peak::graphics::EK_F1;
		case GLFW_KEY_F2:
			return peak::graphics::EK_F2;
		case GLFW_KEY_F3:
			return peak::graphics::EK_F3;
		case GLFW_KEY_F4:
			return peak::graphics::EK_F4;
		case GLFW_KEY_F5:
			return peak::graphics::EK_F5;
		case GLFW_KEY_F6:
			return peak::graphics::EK_F6;
		case GLFW_KEY_F7:
			return peak::graphics::EK_F7;
		case GLFW_KEY_F8:
			return peak::graphics::EK_F8;
		case GLFW_KEY_F9:
			return peak::graphics::EK_F9;
		case GLFW_KEY_F10:
			return peak::graphics::EK_F10;
		case GLFW_KEY_F11:
			return peak::graphics::EK_F11;
		case GLFW_KEY_F12:
			return peak::graphics::EK_F12;
		case GLFW_KEY_UP:
			return peak::graphics::EK_Up;
		case GLFW_KEY_DOWN:
			return peak::graphics::EK_Down;
		case GLFW_KEY_LEFT:
			return peak::graphics::EK_Left;
		case GLFW_KEY_RIGHT:
			return peak::graphics::EK_Right;
		case GLFW_KEY_LSHIFT:
		case GLFW_KEY_RSHIFT:
			return peak::graphics::EK_Shift;
		case GLFW_KEY_LCTRL:
		case GLFW_KEY_RCTRL:
			return peak::graphics::EK_Control;
		case GLFW_KEY_LALT:
		case GLFW_KEY_RALT:
			return peak::graphics::EK_Alt;
		case GLFW_KEY_TAB:
			return peak::graphics::EK_Tab;
		case GLFW_KEY_ENTER:
			return peak::graphics::EK_Return;
		case GLFW_KEY_BACKSPACE:
			return peak::graphics::EK_Backspace;
		case GLFW_KEY_INSERT:
			return peak::graphics::EK_Insert;
		case GLFW_KEY_DEL:
			return peak::graphics::EK_Delete;
		case GLFW_KEY_PAGEUP:
			return peak::graphics::EK_PageUp;
		case GLFW_KEY_PAGEDOWN:
			return peak::graphics::EK_PageDown;
		case GLFW_KEY_HOME:
			return peak::graphics::EK_Home;
		case GLFW_KEY_END:
			return peak::graphics::EK_End;
		case GLFW_KEY_KP_0:
		// TODO: KP_n
		case GLFW_KEY_KP_DIVIDE:
		case GLFW_KEY_KP_MULTIPLY:
		case GLFW_KEY_KP_SUBTRACT:
		case GLFW_KEY_KP_ADD:
		case GLFW_KEY_KP_DECIMAL:
		case GLFW_KEY_KP_EQUAL:
		case GLFW_KEY_KP_ENTER:
			// TODO
			return peak::graphics::EK_None;
		default:
			if ((key >= 'A' && key <= 'Z')
				|| (key >= '0' && key <= 9))
				return (peak::graphics::KeyCode)key;
			return peak::graphics::EK_None;
	}
}

static void GLFWCALL mouseInput(int x, int y)
{
	int dx = x - oldx;
	int dy = y - oldy;
	graphics.injectMouseMovement(x, y, dx, dy);
	oldx = x;
	oldy = y;
}
static void GLFWCALL mouseButtonInput(int key, int state)
{
	graphics.injectMouseButton(key, state);
}
static void GLFWCALL keyboardInput(int key, int state)
{
	if (key == GLFW_KEY_RCTRL && state == GLFW_PRESS)
	{
		mouselocked = !mouselocked;
		if (mouselocked)
			glfwDisable(GLFW_MOUSE_CURSOR);
		else
			glfwEnable(GLFW_MOUSE_CURSOR);
	}
	graphics.injectKeyboard(convertKey(key), state);
}
static void GLFWCALL charInput(int c, int state)
{
	if (state == GLFW_PRESS)
		graphics.injectChar(c);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <game directory>" << std::endl;
		return -1;
	}
	// Create window
	glfwInit();
	if (!glfwOpenWindow(1024, 768, 8, 8, 8, 8, 24, 8, GLFW_WINDOW))
	{
		glfwTerminate();
		std::cerr << "Could not create render window." << std::endl;
		return -1;
	}
	glfwGetMousePos(&oldx, &oldy);
	glfwSetMousePosCallback(mouseInput);
	glfwSetMouseButtonCallback(mouseButtonInput);
	glfwSetKeyCallback(keyboardInput);
	glfwSetCharCallback(charInput);
	glfwDisable(GLFW_MOUSE_CURSOR);
	// Create engine
	peak::Engine engine;
	engine.setDirectory(argv[1]);
	// Initialize graphics
	graphics.setEngine(&engine);
	if (!graphics.init(1024, 768))
	{
		glfwTerminate();
		std::cerr << "Could not initialize graphics." << std::endl;
		return -1;
	}
	/*peak::graphics::CameraSceneNode *camera = new peak::graphics::CameraSceneNode(&graphics,
		"pipelines/deferred.pipeline.xml");
	camera->setParent(graphics.getRootSceneNode());
	camera->setPosition(peak::Vector3F(0, 0, 10));
	camera->setRotation(peak::Vector3F(0, 10, 0));
	graphics.setDefaultCamera(camera);*/
	// Create game
	peak::XMLGame *game = new peak::XMLGame();
	network.registerComponents(game);
	graphics.registerComponents(game);
	physics.registerComponents(game);
	engine.setGame(game);
	if (!game->load())
	{
		std::cerr << "Could not load the game." << std::endl;
		return -1;
	}
	/*// Initialize networking
	peak::WorldComponent *worldcomponent = 0;
	bool isserver = false;
	bool isclient = false;
	peak::network::ClientWorldComponent *client = 0;
	peak::network::ServerWorldComponent *server = new peak::network::ServerWorldComponent(0);
	if (server->init(new peak::Buffer()))
	{
		worldcomponent = server;
		isserver = true;
	}
	else
	{
		delete server;
		client = new peak::network::ClientWorldComponent(0);
		if (!client->init("localhost", 27272))
		{
			delete client;
			glfwTerminate();
			std::cerr << "Could not initialize networking." << std::endl;
			return -1;
		}
		worldcomponent = client;
		isclient = true;
	}
	peak::XMLWorld *world = new peak::XMLWorld(&engine,
		engine.getDirectory() + "/Data/Worlds/Test.xml");
	worldcomponent->setWorld(world);
	world->addComponent(worldcomponent);
	if (!world->load())
	{
		delete world;
		std::cerr << "Could not create world." << std::endl;
		return -1;
	}
	if (isclient)
		client->setReady();*/
	// Create initial world
	peak::World *world = game->createInitialWorld();
	if (!world)
	{
		std::cerr << "Could not create world." << std::endl;
		return -1;
	}
	engine.addWorld(world);
	world->start();
	/*if (isserver)
	{
		peak::Entity *entity = game->getEntityFactory("Player")->createEntity(world,
			(1 << peak::EEF_Server) | (1 << peak::EEF_Local));
		world->addEntity(entity);
	}*/
	// Wait for engine to stop
	unsigned int frames = 0;
	uint64_t lasttime = peak::OS::getSystemTime();
	peak::FrameLimiter limiter(20000);
	while (true)
	{
		engine.update();
		if (!engine.isRunning())
			break;
		if (!graphics.render())
			break;
		glfwSwapBuffers();
		if (glfwGetWindowParam(GLFW_OPENED) != GL_TRUE)
			break;
		// Update frame counter
		frames++;
		if (frames == 10)
		{
			uint64_t time = peak::OS::getSystemTime();
			unsigned int dt = time - lasttime;
			float fps = 1000000.0f / ((float)(time - lasttime) / 10);
			lasttime = time;
			frames = 0;
			char title[64];
			snprintf(title, 64, "PeakEngine - %d fps (%d ms)", (int)fps, dt / 1000);
			glfwSetWindowTitle(title);
		}
		// Only render at 50 fps
		limiter.wait();
	}
	// Destroy the engine
	engine.stop(true);
	if (!game->shutdown())
	{
		std::cerr << "Could not destroy the game." << std::endl;
	}
	delete game;
	// Close graphics
	graphics.shutdown();
	glfwEnable(GLFW_MOUSE_CURSOR);
	glfwTerminate();
	return 0;
}
