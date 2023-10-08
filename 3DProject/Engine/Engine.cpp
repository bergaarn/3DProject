#include "Engine.h"

Engine::Engine():
	renderer(),	sceneHandler(this->window, this->renderer)	
{
	// Initiate All Resources
	this->window.init(1280, 720, "Window");
	this->renderer.init(this->window);

	// Set Main Camera Scene
}

Engine::~Engine()
{
}

void Engine::run()
{
	float frameTime = 0.0f;
	float deltaTime = 0.0f;
	float fps = 0.0f;
	unsigned int frameCount = 0;
	std::chrono::duration<float> timer;
	
	while (this->window.isRunning())
	{
		// Start deltaTime
		auto start = std::chrono::steady_clock::now();
	
		// Prepare scene swaps
		this->sceneHandler.updateScene();
		this->sceneHandler.update();
		this->renderer.render(*this->sceneHandler.getCurrentScene());

		// End deltaTime
		auto end = std::chrono::steady_clock::now();
		timer = end - start;

		// Frame Statistics
		deltaTime = timer.count();
		frameTime += deltaTime;
		frameCount++;
		
		// Update Title Window
		this->window.setTitle(
			"Frame Time: " + std::to_string(frameTime) + " | " +
			"Delta Time: " + std::to_string(deltaTime) + " | " +
			"Frame Count: " + std::to_string(frameCount) + " | "
			"Frames Per Second: " + std::to_string((int)(frameCount/frameTime))
		);

		this->renderer.updateMatrices(frameTime);
	}
	
}