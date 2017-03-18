#include "ProjectManager.h"

GameEngine::ProjectManager::ProjectManager()
	: _activeProject(nullptr) {}

GameEngine::ProjectManager::~ProjectManager() {}

GameEngine::ProjectManager* GameEngine::ProjectManager::instance()
{
	static ProjectManager instance;
	return &instance;
}

GameEngine::Project* GameEngine::ProjectManager::activeProject() const
{
	return _activeProject;
}

void GameEngine::ProjectManager::loadProject(Project* project)
{
	if (_activeProject)
		delete _activeProject;
	_activeProject = project;
}

void GameEngine::ProjectManager::loadProject(const QString& path)
{
	throw std::logic_error("Not implemented");
}
