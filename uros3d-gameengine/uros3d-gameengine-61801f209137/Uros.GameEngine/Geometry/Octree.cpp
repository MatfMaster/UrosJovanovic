#include <QTime>
#include "Ray3D.h"
#include "Octree.h"
#include "OctreeNode.h"
#include "GameObject.h"
#include "Intersect.h"
#include "Rendering/MeshRenderer.h"

GameEngine::Octree::Octree()
	: OctreeNode(nullptr, QVector3D(), 0),
	  _initialized(false) {}

GameEngine::Octree::~Octree() {}

bool GameEngine::Octree::findGameObject(GameObject* gameObject, QVector<OctreeNode*>& nodes) const
{
	auto it = _mapping.constFind(gameObject);
	if (it != _mapping.constEnd())
	{
		nodes = *it;
		return true;
	}
	return false;
}

bool GameEngine::Octree::raycast(const Ray3D& ray, GameObject*& gameObject, QVector3D* hitPoint) const
{
	gameObject = nullptr;
	QMap<float, GameObject*> candidates;
	QMap<float, const OctreeNode*> leafNodes;
	getIntersectingLeafNodes(ray, leafNodes);
	if (leafNodes.count() > 0)
	{
		for (auto leafNode : leafNodes)
			for (auto gObject : leafNode->gameObjects())
				if (auto meshRenderer = gObject->getComponent<MeshRenderer>())
				{
					float t;
					if (Intersect::rayAndAABB(ray, meshRenderer->boundingBox(), &t))
						candidates.insert(t, gObject);
				}
	}
	// Don't forget them outliers...
	for (auto gObject : _outliers)
		if (auto meshRenderer = gObject->getComponent<MeshRenderer>())
		{
			float t;
			if (Intersect::rayAndAABB(ray, meshRenderer->boundingBox(), &t))
				candidates.insert(t, gObject);
		}
	for (auto gObject : candidates)
	{
		float t;
		//TODO: Use colliders here when implemented
		QMatrix4x4 mat = gObject->transform()->getMatrix();
		auto mesh = gObject->getComponent<MeshRenderer>()->getMesh();
		for (int j = 0; j < mesh->triangleCount(); j++)
		{
			QVector3D t0, t1, t2, n0, n1, n2;
			mesh->getTriangleData(j, t0, t1, t2, n0, n1, n2);
			t0 = mat * t0;
			t1 = mat * t1;
			t2 = mat * t2;
			if (Intersect::rayAndTriangle(ray, t0, t1, t2, &t))
			{
				gameObject = gObject;
				*hitPoint = ray.origin() + ray.direction() * t;
				return gameObject;
			}
		}
	}
	return false;
}

void GameEngine::Octree::intersect(const CameraFrustum& frustum, QList<GameObject*>& gameObjects) const
{
	intersectProtected(frustum, gameObjects);
	for (auto outlier : _outliers)
		if (Intersect::frustumAndAABB(frustum, outlier->boundingBox()))
			gameObjects.push_back(outlier);
}

void GameEngine::Octree::add(GameObject* gameObject)
{
	if (!_initialized)
		return;
	QVector<OctreeNode*> nodes;
	addProtected(gameObject, nodes);
	if (nodes.count() > 0)
		_mapping.insert(gameObject, nodes);
	else
		_outliers.insert(gameObject);
}

void GameEngine::Octree::remove(GameObject* gameObject)
{
	if (!_initialized)
		return;
	removeProtected(gameObject);
	_mapping.remove(gameObject);
	_outliers.remove(gameObject);
}

void GameEngine::Octree::update(GameObject* gameObject)
{
	if (!_initialized)
		return;
	remove(gameObject);
	add(gameObject);
}

void GameEngine::Octree::initialize(const QVector<GameObject*>& gameObjects)
{
	float x, y, z;
	float xMin, yMin, zMin;
	float xMax, yMax, zMax;
	xMin = yMin = zMin = std::numeric_limits<float>::max();
	xMax = yMax = zMax = std::numeric_limits<float>::lowest();
	for (auto gameObject : gameObjects)
	{
		auto min = gameObject->boundingBox().minPoint();
		auto max = gameObject->boundingBox().maxPoint();
		x = min[0];
		y = min[1];
		z = min[2];
		xMin = x < xMin ? x : xMin;
		yMin = y < yMin ? y : yMin;
		zMin = z < zMin ? z : zMin;
		xMax = x > xMax ? x : xMax;
		yMax = y > yMax ? y : yMax;
		zMax = z > zMax ? z : zMax;
		x = max[0];
		y = max[1];
		z = max[2];
		xMin = x < xMin ? x : xMin;
		yMin = y < yMin ? y : yMin;
		zMin = z < zMin ? z : zMin;
		xMax = x > xMax ? x : xMax;
		yMax = y > yMax ? y : yMax;
		zMax = z > zMax ? z : zMax;
	}
	initialize(gameObjects, QVector3D(xMin, yMin, zMin), QVector3D(xMax, yMax, zMax));
}

void GameEngine::Octree::initialize(const QVector<GameObject*>& gameObjects, const QVector3D& min, const QVector3D& max)
{
	if (_initialized)
	{
		ERROR_LOG("> Octree::initialize: Octree is already initialized.");
		return;
	}

	QTime timer;
	timer.start();

	auto mid = (min + max) / 2;
	auto extent = max - min;
	init(mid, fmax(extent.z(), fmax(extent.x(), extent.y())));

	_initialized = true;

	for (auto i = 0; i < gameObjects.count(); i++)
		add(gameObjects[i]);

	DEBUG_LOG("> Octree::initialize: took " << timer.elapsed() / 1000.0f << "s");
}
