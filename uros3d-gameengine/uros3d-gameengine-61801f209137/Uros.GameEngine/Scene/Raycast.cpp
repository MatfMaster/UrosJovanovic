#include "Raycast.h"
#include "ProjectManager.h"

GameEngine::Raycast::Raycast(const Ray3D& ray)
{
	_ray = ray;
	_gameObject = nullptr;
	_gameObject = doRaycast();
}

GameEngine::Raycast::~Raycast() {}

bool GameEngine::Raycast::hit() const
{
	return _gameObject;
}

GameEngine::GameObject* GameEngine::Raycast::gameObject() const
{
	return _gameObject;
}

const QVector3D& GameEngine::Raycast::hitPoint() const
{
	return _hitPoint;
}

GameEngine::GameObject* GameEngine::Raycast::doRaycast()
{
	if (auto project = ProjectManager::instance()->activeProject())
	{
		if (auto scene = project->getActiveScene())
		{
			scene->octree().raycast(_ray, _gameObject, &_hitPoint);
			return _gameObject;
		}
		else
			throw std::logic_error("Raycast::intersect: No active scene loaded.");
	}
	else
		throw std::logic_error("Raycast::intersect: No active project loaded.");
}