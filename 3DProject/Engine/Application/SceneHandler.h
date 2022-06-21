#pragma once
#include "Scene.h"
#include "Window.h"

class SceneHandler
{
private:
	Scene* currentScene;
	Scene* nextScene;
	
	Window& window;

public:
	SceneHandler(Window& window);
	~SceneHandler();

	void update();
	void updateScene();
	void setNextScene(Scene* nextScene);
	Scene* getCurrentScene() const;

	inline Window& getWindow() const { return this->window; }

};