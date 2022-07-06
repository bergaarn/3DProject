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
	float frameTime = 0.0f;
	float deltaTime = 0.0f;
	unsigned int frameCount = 0;
	std::chrono::duration<float> timer;
	
	while (this->window.isRunning())
	{
		// Start deltaTime
		auto start = std::chrono::steady_clock::now();
	
		// Prepare scene swaps
		this->sceneHandler.updateScene();
		this->sceneHandler.update();

		// End deltaTime
		auto end = std::chrono::steady_clock::now();
		timer = end - start;

		// Frame Statistics
		//deltaTime = timer.count();
		frameTime += timer.count();
		frameCount++;
		
		/*
		
		// Update Title Window
		this->window.setTitle(
			"Delta Time: " + std::to_string(deltaTime) + " | " +
			"Frame Time: " + std::to_string(frameTime) + " | " +
			"Frame Count: " + std::to_string(frameCount)
		);
		*/


		// Update Title Window
		this->window.setTitle(
			"Frame Time: " + std::to_string(frameTime) + " | " +
			"Frame Count: " + std::to_string(frameCount)
		);
	}
	
}