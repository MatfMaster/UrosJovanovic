#pragma once
#include <QSet>
#include <QVector>
#include <QVector3D>
#include "Includes.h"
#include "BoundingBox.h"

namespace GameEngine {
	class GameObject;
	class Ray3D;
	class CameraFrustum;
	class OctreeNode
	{
		NOCOPY(OctreeNode)

	public:
		const BoundingBox& boundingBox() const;
		const QSet<GameObject*>& gameObjects() const;

	protected:
		OctreeNode(OctreeNode* parent, const QVector3D& center, float size);
		~OctreeNode();
		int level() const;
		bool isLeaf() const;
		OctreeNode* parent() const;
		const QVector<OctreeNode*>& children() const;
		void getIntersectingLeafNodes(const Ray3D& ray, QMap<float, const OctreeNode*>& nodes) const;
		void intersectProtected(const CameraFrustum& frustum, QList<GameObject*>& gameObjects) const;
		void addProtected(GameObject* gameObject, QVector<OctreeNode*>& nodes);
		void removeProtected(GameObject* gameObject);
		void init(const QVector3D& center, float size);
		void clear();

	private:
		int _level;
		OctreeNode* _parent;
		QVector<OctreeNode*> _children;
		QSet<GameObject*> _gameObjects;
		BoundingBox _bbox;
	};
}
