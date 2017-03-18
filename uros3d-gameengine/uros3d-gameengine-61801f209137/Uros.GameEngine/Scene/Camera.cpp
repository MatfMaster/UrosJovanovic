#include <QMatrix4x4>
#include "Camera.h"
#include "GameObject.h"
#include "Application.h"
#include "Rendering/Viewport.h"

GameEngine::Camera::Camera(GameObject* gameObject)
	: Component(gameObject),
	  _enabled(true),
	  _ortho(false),
	  _fov(60),
	  _nearZ(0.1),
	  _farZ(1000),
	  _size(1),
	  _ratio(4.0 / 3.0),
	  _zLayer(0),
	  _frustum(this) {}

GameEngine::Camera::~Camera() {}

GameEngine::GameObject* GameEngine::Camera::create(bool orthographic)
{
	auto gameObject = new GameObject("Camera");
	auto camera = gameObject->addComponent<Camera>();
	if (orthographic)
		camera->enableOrthographicMode();
	else
		camera->disableOrthographicMode();
	return gameObject;
}

GameEngine::Component::ComponentType GameEngine::Camera::type() const
{
	return T_CAMERA;
}

bool GameEngine::Camera::isSingular() const
{
	return true;
}

bool GameEngine::Camera::isEnabled() const
{
	return _enabled;
}

bool GameEngine::Camera::isOrthographic() const
{
	return _ortho;
}

float GameEngine::Camera::getFOV() const
{
	return _fov;
}

float GameEngine::Camera::getClippingNear() const
{
	return _nearZ;
}

float GameEngine::Camera::getClippingFar() const
{
	return _farZ;
}

float GameEngine::Camera::getSize() const
{
	return _size;
}

float GameEngine::Camera::getAspectRatio() const
{
	return _ratio;
}

float GameEngine::Camera::getZLayer() const
{
	return _zLayer;
}

QMatrix4x4 GameEngine::Camera::projectionMatrix() const
{
	QMatrix4x4 proj;
	proj.setToIdentity();
	if (_ortho)
		proj.ortho(-_size / 2 * _ratio, _size / 2 * _ratio, -_size / 2, _size / 2, _nearZ, _farZ);
	else
		proj.perspective(_fov, _ratio, _nearZ, _farZ);

	const QVector3D& p = gameObject()->transform()->getPosition();
	const QVector3D& t = p + gameObject()->transform()->getForward();
	const QVector3D& up = gameObject()->transform()->getUp();

	proj.lookAt(p, t, up);

	return proj;
}

const GameEngine::SkyBox& GameEngine::Camera::getSkyBox() const
{
	return _skyBox;
}

const GameEngine::CameraFrustum& GameEngine::Camera::frustum() const
{
	return _frustum;
}

void GameEngine::Camera::enable()
{
	_enabled = true;
}

void GameEngine::Camera::disable()
{
	_enabled = false;
}

void GameEngine::Camera::enableOrthographicMode()
{
	_ortho = true;
}

void GameEngine::Camera::disableOrthographicMode()
{
	_ortho = false;
}

void GameEngine::Camera::setFOV(float fov)
{
	_fov = fov;
}

void GameEngine::Camera::setClippingNear(float nearZ)
{
	_nearZ = nearZ;
}

void GameEngine::Camera::setClippingFar(float farZ)
{
	_farZ = farZ;
}

void GameEngine::Camera::setSize(float size)
{
	if (size < 1)
		size = 1;
	_size = size;
}

void GameEngine::Camera::setAspectRatio(float ratio)
{
	_ratio = ratio;
}

void GameEngine::Camera::setZLayer(float zLayer)
{
	_zLayer = zLayer;
}

void GameEngine::Camera::setSkyBox(const SkyBox& skyBox)
{
	_skyBox = skyBox;
}

QVector3D GameEngine::Camera::screenToWorld(const QVector3D& screen) const
{
	// Based on gluUnproject code at
	// https://www.opengl.org/wiki/GluProject_and_gluUnProject_code

	auto viewport = Application::viewport();
	float left = viewport->position().x();
	float top = viewport->position().y();
	float winX = screen.x() - left;
	float winY = screen.y() - top;
	float winZ = screen.z();
	float width = viewport->width();
	float height = viewport->height();

	winY = height - winY; // 0,0 is top, left corner in a window but lower left corner in viewport

	// Compute the inverse of the (perspective x model-view) matrix.
	auto transformMatrix = projectionMatrix().inverted();

	// Transformation of normalized coordinates (-1 to 1).
	QVector4D inVector
		(
		winX / width * 2 - 1.0,
		winY / height * 2 - 1.0,
		2 * winZ - 1.0,
		1.0
		);

	// Now transform that vector into object coordinates.
	QVector4D resultVec = transformMatrix * inVector;

	// Invert to normalize x, y, and z values.
	resultVec[3] = 1.0f / resultVec[3];

	float x = resultVec[0] * resultVec[3];
	float y = resultVec[1] * resultVec[3];
	float z = resultVec[2] * resultVec[3];

	return QVector3D(x, y, z);
}

QVector3D GameEngine::Camera::worldToScreen(const QVector3D& world) const
{
	// TODO
	return QVector3D();
}

GameEngine::CameraFrustum::CameraFrustum(Camera* camera)
{
	_camera = camera;
}

GameEngine::Camera* GameEngine::CameraFrustum::camera() const
{
	return _camera;
}

GameEngine::BoundingBox GameEngine::CameraFrustum::boundingBox() const
{
	QVector<QVector3D> corners(8);
	getCorners(corners);
	return BoundingBox::create(corners);
}

void GameEngine::CameraFrustum::getCorners(QVector<QVector3D>& corners) const
{
	//Extracting frustum corners
	//http://gamedev.stackexchange.com/questions/19774/determine-corners-of-a-specific-plane-in-the-frustum

	const float PI = 3.1415926;
	Transform* transform = _camera->gameObject()->transform();
	QVector3D p = transform->getPosition();
	QVector3D f = transform->getForward();
	QVector3D up = transform->getUp();
	QVector3D r = transform->getRight();
	float near = _camera->getClippingNear();
	float far = _camera->getClippingFar();
	float aspect = _camera->getAspectRatio();

	float nearHeight;
	float nearWidth;
	float farHeight;
	float farWidth;
	QVector3D centerNear = p + f * near;
	QVector3D centerFar = p + f * far;

	if (_camera->isOrthographic())
	{
		float size = _camera->getSize();
		float left = -size / 2 * aspect;
		float right = size / 2 * aspect;
		float bottom = -size / 2;
		float top = size / 2;
		nearHeight = farHeight = fabs(top - bottom);
		nearWidth = farWidth = fabs(right - left);
	}
	else
	{
		float fov = _camera->getFOV();
		nearHeight = 2 * tanf(fov * PI / 180 / 2) * near;
		nearWidth = nearHeight * aspect;
		farHeight = 2 * tanf(fov * PI / 180 / 2) * far;
		farWidth = farHeight * aspect;
	}

	// Near
	corners[0] = centerNear - r * (nearWidth / 2) - up * (nearHeight / 2); // Bottom left
	corners[1] = centerNear + r * (nearWidth / 2) - up * (nearHeight / 2); // Bottom right
	corners[2] = centerNear + r * (nearWidth / 2) + up * (nearHeight / 2); // Top right
	corners[3] = centerNear - r * (nearWidth / 2) + up * (nearHeight / 2); // Top left

	// Far
	corners[4] = centerFar - r * (farWidth / 2) - up * (farHeight / 2); // Bottom left
	corners[5] = centerFar + r * (farWidth / 2) - up * (farHeight / 2); // Bottom right
	corners[6] = centerFar + r * (farWidth / 2) + up * (farHeight / 2); // Top right
	corners[7] = centerFar - r * (farWidth / 2) + up * (farHeight / 2); // Top left
}

void GameEngine::CameraFrustum::getSegments(QVector<Segment3D>& segments) const
{
	static QVector<QVector3D> corners(8);
	getCorners(corners);

	segments[0] = Segment3D(corners[0], corners[1]);
	segments[1] = Segment3D(corners[1], corners[2]);
	segments[2] = Segment3D(corners[2], corners[3]);
	segments[3] = Segment3D(corners[3], corners[0]);

	segments[4] = Segment3D(corners[4], corners[5]);
	segments[5] = Segment3D(corners[5], corners[6]);
	segments[6] = Segment3D(corners[6], corners[7]);
	segments[7] = Segment3D(corners[7], corners[4]);

	segments[8] = Segment3D(corners[0], corners[4]);
	segments[9] = Segment3D(corners[1], corners[5]);
	segments[10] = Segment3D(corners[2], corners[6]);
	segments[11] = Segment3D(corners[3], corners[7]);
}

void GameEngine::CameraFrustum::getPlanes(QVector<Plane3D>& planes) const
{
	static QVector<QVector3D> corners(8);
	getCorners(corners);

	planes[0] = Plane3D(corners[0], corners[1], corners[2]); // Front
	planes[1] = Plane3D(corners[5], corners[4], corners[7]); // Back
	planes[2] = Plane3D(corners[4], corners[0], corners[3]); // Left
	planes[3] = Plane3D(corners[1], corners[5], corners[6]); // Right
	planes[4] = Plane3D(corners[7], corners[3], corners[2]); // Top
	planes[5] = Plane3D(corners[0], corners[4], corners[5]); // Bottom
}
