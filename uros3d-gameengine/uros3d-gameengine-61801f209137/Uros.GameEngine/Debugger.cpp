#include "Debugger.h"
#include "GameObject.h"
#include "Geometry/Segment3D.h"
#include "Geometry/OctreeNode.h"
#include "Rendering/RenderingManager.h"
#include "Scene/Scene.h"
#include "Scene/Light.h"
#include "Scene/Camera.h"

GameEngine::Debugger::~Debugger() {}

GameEngine::Debugger::Debugger(GameObject* gameObject)
	: Component(gameObject),
	  _gizmo(true),
	  _octree(true),
	  _bbox(true) { }

GameEngine::Component::ComponentType GameEngine::Debugger::type() const
{
	return T_DEBUGGER;
}

bool GameEngine::Debugger::isSingular() const
{
	return true;
}

bool GameEngine::Debugger::isGizmoVisible() const
{
	return _gizmo;
}

bool GameEngine::Debugger::isOctreeNodeVisible() const
{
	return _octree;
}

bool GameEngine::Debugger::isBoundingBoxVisible() const
{
	return _bbox;
}

void GameEngine::Debugger::showGizmo()
{
	_gizmo = true;
}

void GameEngine::Debugger::showOctreeNode()
{
	_octree = true;
}

void GameEngine::Debugger::showBoundingBox()
{
	_bbox = true;
}

void GameEngine::Debugger::hideGizmo()
{
	_gizmo = false;
}

void GameEngine::Debugger::hideOctreeNode()
{
	_octree = false;
}

void GameEngine::Debugger::hideBoundingBox()
{
	_bbox = false;
}

void GameEngine::Debugger::render(const Scene* scene) const
{
	RenderingManagerInstance* renderingManager
		= RenderingManager::instance();

	if (_gizmo)
	{
		Transform* transform = gameObject()->transform();
		QVector3D position = transform->getPosition();

		Segment3D segment = Segment3D(position, position + transform->getUp());
		renderingManager->dbgDrawLines(&segment, 1, Qt::green);

		segment = Segment3D(position, position + transform->getRight());
		renderingManager->dbgDrawLines(&segment, 1, Qt::red);

		segment = Segment3D(position, position + transform->getForward());
		renderingManager->dbgDrawLines(&segment, 1, Qt::blue);
	}

	QVector<Segment3D> segments(12);
	if (_octree)
	{
		QVector<OctreeNode*> nodes;
		if (scene->octree().findGameObject(gameObject(), nodes))
		{
			for (int i = 0; i < nodes.count(); i++)
			{
				auto node = nodes[i];

				node->boundingBox().getSegments(segments);
				renderingManager->dbgDrawLines(segments.constData(), segments.count(), Qt::white);
			}
		}
	}

	if (_bbox)
	{
		if (auto camera = gameObject()->getComponent<Camera>())
			camera->frustum().getSegments(segments);
		else if (auto light = gameObject()->getComponent<Light>())
		{
			auto transform = light->gameObject()->transform();
			if (light->getType() == Light::Point)
			{
				Segment3D::approximateCircle(transform->getPosition(), transform->getUp(), light->getRange(), 36, segments);
				Segment3D::approximateCircle(transform->getPosition(), transform->getRight(), light->getRange(), 36, segments);
				Segment3D::approximateCircle(transform->getPosition(), transform->getForward(), light->getRange(), 36, segments);
			}
		}
		else
			gameObject()->boundingBox().getSegments(segments);
		renderingManager->dbgDrawLines(segments.constData(), segments.count(), Qt::yellow);
	}
}