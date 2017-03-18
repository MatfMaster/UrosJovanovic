#include "Project.h"

GameEngine::Project::Project()
	: _name(""),
	  _activeScene(nullptr) {}

GameEngine::Project::Project(const char* name)
	: _name(name),
	  _activeScene(nullptr) {}

GameEngine::Project::~Project()
{
	delete _activeScene;
}

const QString& GameEngine::Project::getName() const
{
	return _name;
}

void GameEngine::Project::setName(const QString& name)
{
	_name = name;
}

GameEngine::Scene* GameEngine::Project::getActiveScene() const
{
	return _activeScene;
}

void GameEngine::Project::setActiveScene(Scene* scene)
{
	if (_activeScene)
		delete _activeScene;
	_activeScene = scene;
}
