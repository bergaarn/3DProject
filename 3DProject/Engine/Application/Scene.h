#pragma once

class SceneHandler;
class Window;

class Scene 
{
private:
	Window* window;
	SceneHandler& sceneHandler;

protected:
	inline Window& getWindow() { return *this->window; }
	inline SceneHandler& getSceneHandler() { return this->sceneHandler; }

public:
	Scene(SceneHandler& sceneHandler);
	~Scene();

	void init();
	void update();
};