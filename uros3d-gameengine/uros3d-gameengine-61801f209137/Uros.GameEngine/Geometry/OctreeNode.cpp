#include "OctreeNode.h"
#include "GameObject.h"
#include "Intersect.h"
#include "Scene/Camera.h"

#define MAX_DEPTH 4

GameEngine::OctreeNode::OctreeNode(OctreeNode* parent, const QVector3D& center, float size)
	: _level(parent ? parent->level() + 1 : 0),
	  _parent(parent)
{
	init(center, size);
}

GameEngine::OctreeNode::~OctreeNode()
{
	clear();
}

const GameEngine::BoundingBox& GameEngine::OctreeNode::boundingBox() const
{
	return _bbox;
}

void GameEngine::OctreeNode::intersectProtected(const CameraFrustum& frustum, QList<GameObject*>& gameObjects) const
{
	const auto& frustumBox = frustum.boundingBox();
	if (Intersect::aabbAndAABB(frustumBox, _bbox))
		if (Intersect::frustumAndAABB(frustum, _bbox))
		{
			if (!isLeaf())
				for (auto i = 0; i < _children.count(); i++)
					_children[i]->intersectProtected(frustum, gameObjects);
			else
				for (const auto& gameObject : _gameObjects)
					//if (Intersect::frustumAndAABB(frustum, gameObject->boundingBox())) <-- too slow
						if (!gameObject->isStatic())
							gameObjects.push_back(gameObject);
		}
}

int GameEngine::OctreeNode::level() const
{
	return _level;
}

bool GameEngine::OctreeNode::isLeaf() const
{
	return _level == MAX_DEPTH - 1;
}

GameEngine::OctreeNode* GameEngine::OctreeNode::parent() const
{
	return _parent;
}

const QVector<GameEngine::OctreeNode*>& GameEngine::OctreeNode::children() const
{
	return _children;
}

const QSet<GameEngine::GameObject*>& GameEngine::OctreeNode::gameObjects() const
{
	return _gameObjects;
}

void GameEngine::OctreeNode::getIntersectingLeafNodes(const Ray3D& ray, QMap<float, const OctreeNode*>& nodes) const
{
	float t;
	if (Intersect::rayAndAABB(ray, boundingBox(), &t))
		if (isLeaf())
			nodes.insert(t, this);
		else
			for (auto i = 0; i < _children.count(); i++)
				_children[i]->getIntersectingLeafNodes(ray, nodes);
}

void GameEngine::OctreeNode::addProtected(GameObject* gameObject, QVector<OctreeNode*>& nodes)
{
	auto gameObjectBox = gameObject->boundingBox();
	if (Intersect::aabbAndAABB(_bbox, gameObjectBox))
		if (!isLeaf())
			for (auto i = 0; i < _children.count(); i++)
				_children[i]->addProtected(gameObject, nodes);
		else
		{
			if (BoundingBox::isInsideOf(_bbox, gameObjectBox))
			{
				_gameObjects.insert(gameObject);
				nodes.push_back(this);
			}
			else
			{
				// Boxes intersect, do triangle level intersection
				// EDIT: Maybe not... this is way too slow for larger models
				//if (auto renderer = gameObject->getComponent<Renderer>())
				//	if (auto geom = renderer->geometry())
				//	{
				//		QVector3D v1, v2, v3, n1, n2, n3;
				//		auto mat = gameObject->transform()->getMatrix();
				//		for (auto i = 0; i < geom->triangleCount(); i++)
				//		{
				//			geom->getTriangleData(i, v1, v2, v3, n1, n2, n3);
				//			v1 = mat * v1;
				//			v2 = mat * v2;
				//			v3 = mat * v3;
				//			if (Intersect::triangleAndAABB(v1, v2, v3, _bbox))
				//			{
								_gameObjects.insert(gameObject);
								nodes.push_back(this);
				//				return;
				//			}
				//		}
				//	}
			}
		}
}

void GameEngine::OctreeNode::removeProtected(GameObject* gameObject)
{
	if (isLeaf())
		_gameObjects.remove(gameObject);
	else
		for (auto i = 0; i < _children.count(); i++)
			_children[i]->removeProtected(gameObject);
}

void GameEngine::OctreeNode::init(const QVector3D& center, float size)
{
	clear();
	auto halfSize = size * 0.5f;
	auto min = QVector3D(center.x() - halfSize, center.y() - halfSize, center.z() - halfSize);
	auto max = QVector3D(center.x() + halfSize, center.y() + halfSize, center.z() + halfSize);
	_bbox = BoundingBox(min, max);

	if (!isLeaf())
	{
		auto d = halfSize * 0.5;
		_children =
			{
				new OctreeNode(this, QVector3D(center.x() - d, center.y() - d, center.z() - d), halfSize),
				new OctreeNode(this, QVector3D(center.x() + d, center.y() - d, center.z() - d), halfSize),
				new OctreeNode(this, QVector3D(center.x() - d, center.y() + d, center.z() - d), halfSize),
				new OctreeNode(this, QVector3D(center.x() + d, center.y() + d, center.z() - d), halfSize),

				new OctreeNode(this, QVector3D(center.x() - d, center.y() - d, center.z() + d), halfSize),
				new OctreeNode(this, QVector3D(center.x() + d, center.y() - d, center.z() + d), halfSize),
				new OctreeNode(this, QVector3D(center.x() - d, center.y() + d, center.z() + d), halfSize),
				new OctreeNode(this, QVector3D(center.x() + d, center.y() + d, center.z() + d), halfSize)
			};
	}
}

void GameEngine::OctreeNode::clear()
{
	for (auto child : _children)
		delete child;
	_children.clear();
}
