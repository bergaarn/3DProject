#include "Scene.h"
#include "SceneHandler.h"

Scene::Scene(SceneHandler& sceneHandler):
	window(&sceneHandler.getWindow()), sceneHandler(sceneHandler)
{
}

Scene::~Scene()
{
}

void Scene::init()
{
}

void Scene::update()
{
}
