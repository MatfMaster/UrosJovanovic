#include <QMatrix4x4>
#include "MeshRenderer.h"
#include "GameObject.h"
#include "RenderingManager.h"

GameEngine::MeshRenderer::MeshRenderer(GameObject* gameObject)
	: Renderer(gameObject),
	  _mesh(nullptr),
	  _bbox(nullptr),
	  _frameID(-1) {}

GameEngine::MeshRenderer::~MeshRenderer()
{
	// TODO: Mesh ptr handling
	delete _bbox;
}

const GameEngine::BoundingBox& GameEngine::MeshRenderer::boundingBox()
{
	if (!_bbox)
	{
		if (auto geom = _mesh)
		{
			float x, y, z;
			float xMin, yMin, zMin;
			float xMax, yMax, zMax;
			xMin = yMin = zMin = std::numeric_limits<float>::max();
			xMax = yMax = zMax = std::numeric_limits<float>::lowest();
			QMatrix4x4 transform = gameObject()->transform()->getMatrix();

			QVector<QVector3D> vertices(8);
			geom->boundingBox().getVertices(vertices);
			for (int i = 0; i < vertices.count(); i++)
			{
				auto v = transform * vertices[i];
				x = v[0];
				y = v[1];
				z = v[2];
				xMin = x < xMin ? x : xMin;
				yMin = y < yMin ? y : yMin;
				zMin = z < zMin ? z : zMin;
				xMax = x > xMax ? x : xMax;
				yMax = y > yMax ? y : yMax;
				zMax = z > zMax ? z : zMax;
			}

			_bbox = new BoundingBox(QVector3D(xMin, yMin, zMin), QVector3D(xMax, yMax, zMax));
		}
		else
			_bbox = new BoundingBox(gameObject()->transform()->getPosition(), gameObject()->transform()->getPosition());
	}
	return *_bbox;
}

GameEngine::Mesh* GameEngine::MeshRenderer::getMesh() const
{
	return _mesh;
}

void GameEngine::MeshRenderer::setMesh(Mesh* mesh)
{
	_mesh = mesh;
	delete _bbox;
	_bbox = nullptr;
}

void GameEngine::MeshRenderer::render()
{
	if (!_mesh)
		return;
	auto renderManager = RenderingManager::instance();
	int curFrameID = renderManager->stats().currentFrame().id();
	if (_frameID == curFrameID)
		return; // Already rendered, skip redundant draw calls
	auto transform = gameObject()->transform()->getMatrix();
	renderManager->pushTransform(transform);
	renderManager->bindMaterial(getConstMaterial());
	renderManager->draw(_mesh);
	renderManager->popTransform();
	_frameID = curFrameID;
}

void GameEngine::MeshRenderer::onTransformChanged(Transform* transform)
{
	delete _bbox;
	_bbox = nullptr;
}
