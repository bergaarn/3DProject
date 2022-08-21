#pragma once

#include <chrono>
#include "Application/Window.h"
#include "Application/SceneHandler.h"
#include "Graphics/Renderer.h"

class Engine
{
private:
	Window window;
	SceneHandler sceneHandler;
	Renderer renderer;

	// Resources

public:
	Engine();
	~Engine();

	void run();
};