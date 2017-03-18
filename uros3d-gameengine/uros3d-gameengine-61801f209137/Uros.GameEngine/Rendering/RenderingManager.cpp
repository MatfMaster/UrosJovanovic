#include "RenderingManager.h"
#include "Geometry/Intersect.h"
#include "Scene/Camera.h"

GameEngine::RenderingManagerInstance* GameEngine::RenderingManager::_instance = nullptr;

GameEngine::RenderingManagerInstance::RenderingManagerInstance() {}

GameEngine::RenderingManagerInstance::~RenderingManagerInstance()
{
	for (auto batch : _staticBatches)
		delete batch;
}

void GameEngine::RenderingManagerInstance::drawOpaqueBatches(const Camera* activeCamera)
{
	pushTransform(QMatrix4x4());
	{
		for (auto batch : _staticBatches)
		{
			if (batch->material().getShaderType() < 100)
				continue;

			if (auto geometry = batch->geometry())
			{
				if (!activeCamera || activeCamera &&
					Intersect::aabbAndAABB(activeCamera->frustum().boundingBox(), geometry->boundingBox()) &&
					Intersect::frustumAndAABB(activeCamera->frustum(), geometry->boundingBox()))
				{
					bindMaterial(batch->material());
					draw(geometry);
				}
			}
		}
	}
	popTransform();
}

void GameEngine::RenderingManagerInstance::drawTransparentBatches(const Camera* activeCamera)
{
	pushTransform(QMatrix4x4());
	{
		for (auto batch : _staticBatches)
		{
			auto material = batch->material();
			if (material.getShaderType() >= 100 || material.getOpacity() <= 0)
				continue;

			if (auto geometry = batch->geometry())
			{
				if (!activeCamera || activeCamera &&
					Intersect::aabbAndAABB(activeCamera->frustum().boundingBox(), geometry->boundingBox()) &&
					Intersect::frustumAndAABB(activeCamera->frustum(), geometry->boundingBox()))
				{
					bindMaterial(batch->material());
					draw(geometry);
				}
			}
		}
	}
	popTransform();
}

GameEngine::RenderStats& GameEngine::RenderingManagerInstance::stats()
{
	return _stats;
}

GameEngine::RenderingManagerInstance* GameEngine::RenderingManager::instance()
{
	if (!_instance)
		throw std::logic_error("RenderingManager not initialized.");
	return _instance;
}

void GameEngine::RenderingManager::initialize(RenderingManagerInstance* instance)
{
	if (_instance)
		throw std::logic_error("RenderingManager already initialized.");
	_instance = instance;
}

void GameEngine::RenderingManager::destroy()
{
	delete _instance;
}
