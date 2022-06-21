#pragma once
#include <chrono>
#include "Application/Window.h"
#include "Application/SceneHandler.h"

class Engine
{
private:
	Window window;
	SceneHandler sceneHandler;
	// Renderer
	// Resources

public:
	Engine();
	~Engine();

	void run();
};