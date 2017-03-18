#include "CameraController.h"
#include "GameObject.h"
#include "Scene/Camera.h"
#include "IO/InputManager.h"

using namespace GameEngine;

CameraController::CameraController(GameObject* gameObject)
	: Behaviour(gameObject) {}

void CameraController::update(double deltaTime)
{
	auto camera = gameObject();

	if (Input.getMouseButtonDown() & Qt::LeftButton && !(Input.getModifiers() & Qt::CTRL))
	{
		float angleX = Input.getMouseXAxis() / 2.0;
		float angleY = Input.getMouseYAxis() / 2.0;

		camera->transform()->rotateAround(target, Transform::up(), -angleX);
		camera->transform()->rotateAround(target, camera->transform()->getRight(), -angleY);
	}

	if (Input.getMouseWheelAxis() < 0 || Input.getMouseWheelAxis() != 0 && camera->transform()->getPosition().distanceToPoint(target) >= 1)
	{
		float mouseWheelDelta = Input.getMouseWheelAxis() / 100.0;
		auto cameraComponent = camera->getComponent<Camera>();
		if (cameraComponent->isOrthographic())
			cameraComponent->setSize(cameraComponent->getSize() - mouseWheelDelta);
		camera->transform()->setPosition(camera->transform()->getPosition() + camera->transform()->getForward() * mouseWheelDelta);
	}

	if ((Input.getMouseButtons() & Qt::MiddleButton) == Qt::MiddleButton)
	{
		float deltaX = Input.getMouseXAxis() / 20.0;
		float deltaY = Input.getMouseYAxis() / 20.0;

		camera->transform()->setPosition(camera->transform()->getPosition() - camera->transform()->getUp() * deltaY);
		camera->transform()->setPosition(camera->transform()->getPosition() + camera->transform()->getRight() * deltaX);

		target = target - camera->transform()->getUp() * deltaY;
		target = target + camera->transform()->getRight() * deltaX;
	}

	camera->transform()->lookAt(target, camera->transform()->getUp());
}

const QVector3D& CameraController::getTarget() const
{
	return target;
}

void CameraController::setTarget(const QVector3D& target)
{
	this->target = target;
}
