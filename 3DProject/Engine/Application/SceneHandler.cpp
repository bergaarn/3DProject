#include "SceneHandler.h"

SceneHandler::SceneHandler(Window& window, Renderer& renderer): 
	currentScene(nullptr),
	nextScene(nullptr),
	window(window),
	renderer(renderer)
{
}

SceneHandler::~SceneHandler()
{
	delete this->currentScene;
}

void SceneHandler::update()
{
	this->currentScene->update();
}

void SceneHandler::updateScene()
{
	if (this->nextScene != nullptr)
	{
		delete this->currentScene;
		this->currentScene = nullptr;

		this->currentScene = this->nextScene;
		this->nextScene = nullptr;
		this->currentScene->init();
	}
}

void SceneHandler::setNextScene(Scene* nextScene)
{
	if (this->nextScene == nullptr)
	{
		this->nextScene = nextScene;
	}
}

Scene* SceneHandler::getCurrentScene() const
{
	return this->currentScene;
}