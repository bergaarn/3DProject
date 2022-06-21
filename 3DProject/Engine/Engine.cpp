#include "Engine.h"

Engine::Engine(): sceneHandler(this->window)
{
	// Initiate All Resources
	this->window.init(1280, 720, "Window");


	// Set Main Camera Scene
}

Engine::~Engine()
{
}

void Engine::run()
{
	while (this->window.isRunning())
	{
		// Start deltaTime
		auto start = std::chrono::high_resolution_clock::now();
	
		// Prepare scene swaps
		this->sceneHandler.updateScene();
		this->sceneHandler.update();

		// End deltaTime
		auto end = std::chrono::steady_clock::now();
		std::chrono::duration<float> timer = end - start;
		
		// Update Title Window
		//this->window.setTitle("3D Project | Joel Berg | 2022 | Delta Time: " + std::to_string(timer.count()));
	}
	
}