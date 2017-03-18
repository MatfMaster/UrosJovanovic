#include <QSet>
#include <QElapsedTimer>
#include "Scene.h"
#include "GameObject.h"
#include "Light.h"
#include "Camera.h"
#include "Behaviour.h"
#include "Debugger.h"
#include "Rendering/Renderer.h"
#include "Rendering/RenderingManager.h"

#define FRUSTUM_CULLING
GameEngine::Scene::Scene() {}

GameEngine::Scene::Scene(const QString& name)
	: Scene()
{
	_name = name;
}

GameEngine::Scene::~Scene()
{
	const auto& gameObjects = _gameObjects;
	_gameObjects.clear();
	for (const auto& gameObject : gameObjects)
		GameObject::destroy(gameObject);
}

const QString& GameEngine::Scene::getName() const
{
	return _name;
}

const QList<GameEngine::Light*>& GameEngine::Scene::lights() const
{
	return _lights;
}

const QList<GameEngine::Camera*>& GameEngine::Scene::cameras() const
{
	return _cameras;
}

const QList<GameEngine::GameObject*>& GameEngine::Scene::gameObjects() const
{
	return _gameObjects;
}

GameEngine::GameObject* GameEngine::Scene::findGameObject(const QString& name) const
{
	for (const auto& gameObject : _gameObjects)
		if (gameObject->getName().compare(name) == 0)
			return gameObject;
	return nullptr;
}

QList<GameEngine::GameObject*> GameEngine::Scene::findGameObjects(const QString& name) const
{
	auto result = QList<GameObject*>();
	for (const auto& gameObject : _gameObjects)
		if (gameObject->getName().compare(name) == 0)
			result.push_back(gameObject);
	return result;
}

void GameEngine::Scene::setName(const QString& name)
{
	_name = name;
}

void GameEngine::Scene::initialize()
{
	_octree.initialize(_gameObjects.toVector());
	QVector<const MeshRenderer*> statics;
	for (const auto& gameObject : _gameObjects)
	{
		if (gameObject->isStatic())
		{
			if (const auto& meshRenderer = gameObject->getComponent<MeshRenderer>())
				statics.push_back(meshRenderer);
		}
		else
			if (const auto& renderer = gameObject->getComponent<Renderer>())
				renderer->render();		
	}
	RenderingManager::instance()->buildStaticBatches(statics.constBegin(), statics.constEnd());
	RenderingManager::instance()->drawOpaqueBatches();
	RenderingManager::instance()->drawTransparentBatches();
}

const GameEngine::Octree& GameEngine::Scene::octree() const
{
	return _octree;
}

void GameEngine::Scene::addGameObject(GameObject* gameObject)
{
	connect(gameObject, SIGNAL(componentAdded(GameObject*, Component*)),
		this, SLOT(onComponentAdded(GameObject*, Component*)));
	connect(gameObject, SIGNAL(componentRemoved(GameObject*, Component*)),
		this, SLOT(onComponentRemoved(GameObject*, Component*)));
	connect(gameObject->transform(), SIGNAL(changed(Transform*)),
		this, SLOT(onTransformChanged(Transform*)));
	_gameObjects.push_back(gameObject);
	_octree.add(gameObject);
}

void GameEngine::Scene::removeGameObject(GameObject* gameObject)
{
	disconnect(gameObject, SIGNAL(componentAdded(GameObject*, Component*)),
		this, SLOT(onComponentAdded(GameObject*, Component*)));
	disconnect(gameObject, SIGNAL(componentRemoved(GameObject*, Component*)),
		this, SLOT(onComponentRemoved(GameObject*, Component*)));
	disconnect(gameObject->transform(), SIGNAL(changed(Transform*)),
		this, SLOT(onTransformChanged(Transform*)));
	_gameObjects.removeAll(gameObject);
	_octree.remove(gameObject);
}

void GameEngine::Scene::update(double deltaTime) const
{
	for (const auto& behaviour : _behaviours.toVector())
		if (behaviour && behaviour->isEnabled())
			behaviour->update(deltaTime);
}

void GameEngine::Scene::render()
{
	QElapsedTimer timer;
	timer.start();

	RenderingManagerInstance* renderingManager
		= RenderingManager::instance();

	QList<Camera*> activeCameras;
	for (const auto& camera : _cameras)
		if (camera->isEnabled())
			activeCameras.push_back(camera);
	// Sort cameras in descending order by Z layer
	std::sort(activeCameras.begin(), activeCameras.end(),
	          [](Camera* cam1, Camera* cam2)
	          {
		          return cam1->getZLayer() > cam2->getZLayer();
	          });
	// Camera with the highest Z layer is definitive 'active' camera
	const auto& activeCamera = activeCameras.first();
	renderingManager->setActiveCamera(activeCamera);

	QList<Light*> activeLights;
	for (const auto& light : _lights)
		if (light->isEnabled())
			activeLights.push_back(light);
	renderingManager->setActiveLights(activeLights);

	/* ------------------------ Opaque Objects ------------------------ */

	_transparentObjects.clear();
#ifdef FRUSTUM_CULLING
	renderingManager->stats().setFrustumCullStatus(true);
	int prevCount = _visibleObjects.count();
	_visibleObjects.clear();
	_visibleObjects.reserve(prevCount);
	_octree.intersect(activeCamera->frustum(), _visibleObjects);
	for (const auto& gameObject : _visibleObjects)
#else
	renderingManager->stats().setFrustumCullStatus(false);
	for (const auto& gameObject : _gameObjects)
#endif
	if (!gameObject->isStatic())
	{
		const auto& renderer = gameObject->getComponent<Renderer>();

		if (renderer && renderer->isEnabled())
		{
			const auto& material = renderer->getConstMaterial();
			if (material.getShaderType() >= 100)
			{
				// Transculent objects are rendered last
				// Completely transparent objects are discarded
				if (material.getOpacity() > 0)
					_transparentObjects.push_back(renderer);
				continue;
			}
			renderer->render();
		}
	}

#ifdef FRUSTUM_CULLING
	renderingManager->drawOpaqueBatches(activeCamera);
#else
	renderingManager->drawOpaqueBatches();
#endif

	// Render skybox
	QMatrix4x4 skyBoxTransform;
	skyBoxTransform.translate(activeCamera->gameObject()->transform()->getPosition());
	skyBoxTransform.scale(10, 10, 10);
	renderingManager->pushTransform(skyBoxTransform);
	renderingManager->draw(&activeCamera->getSkyBox());
	renderingManager->popTransform();

	/* ------------------------ Transculent/Transparent Objects ------------------------ */

	for (const auto& transparentObject : _transparentObjects)
		transparentObject->render();

#ifdef FRUSTUM_CULLING
	renderingManager->drawTransparentBatches(activeCamera);
#else
	renderingManager->drawTransparentBatches();
#endif

#if _DEBUG
	for (const auto& debugger : _debuggers)
		debugger->render(this);
#endif

	renderingManager->stats().currentFrame().setTime(timer.nsecsElapsed() * 10e-6);
	renderingManager->stats().pushCurrentFrame();
}

void GameEngine::Scene::onComponentAdded(GameObject* gameObject, Component* component)
{
	if (const auto& light = dynamic_cast<Light*>(component))
		_lights.push_back(light);
	else if (const auto& camera = dynamic_cast<Camera*>(component))
		_cameras.push_back(camera);
	else if (const auto& behaviour = dynamic_cast<Behaviour*>(component))
		_behaviours.push_back(behaviour);
	else if (const auto& debugger = dynamic_cast<Debugger*>(component))
		_debuggers.push_back(debugger);
}

void GameEngine::Scene::onComponentRemoved(GameObject* gameObject, Component* component)
{
	if (const auto& light = dynamic_cast<Light*>(component))
		_lights.removeAll(light);
	else if (const auto& camera = dynamic_cast<Camera*>(component))
		_cameras.removeAll(camera);
	else if (const auto& behaviour = dynamic_cast<Behaviour*>(component))
		_behaviours.removeAll(behaviour);
	else if (const auto& debugger = dynamic_cast<Debugger*>(component))
		_debuggers.removeAll(debugger);
}

void GameEngine::Scene::onTransformChanged(Transform* transform)
{
	_octree.update(transform->gameObject());
}
