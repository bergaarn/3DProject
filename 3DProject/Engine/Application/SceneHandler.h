#pragma once

#include "Scene.h"
#include "Window.h"
#include "../Graphics/Renderer.h"

class SceneHandler
{
private:
	Scene* currentScene;
	Scene* nextScene;
	
	Window& window;
	Renderer& renderer;

public:
	SceneHandler(Window& window, Renderer& renderer);
	~SceneHandler();

	void update();
	void updateScene();
	void setNextScene(Scene* nextScene);
	Scene* getCurrentScene() const;

	inline Window& getWindow() const { return this->window; }

};