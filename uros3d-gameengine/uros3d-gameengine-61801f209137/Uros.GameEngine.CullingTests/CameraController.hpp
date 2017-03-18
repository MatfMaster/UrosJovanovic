#pragma once 
#include "Behaviour.h"
#include "GameObject.h"
#include "IO/InputManager.h"
#include "Scene/Camera.h"
#include "Transform.h"

using namespace GameEngine;

class CameraController : public Behaviour
{
	QVector3D target;
public:
	explicit CameraController(GameObject* gameObject)
		: Behaviour(gameObject) { }

	void update(double deltaTime) override
	{
		auto camera = gameObject();
		auto input = InputManager::instance();

		if (input->getMouseButtonDown() & Qt::LeftButton)
		{
			float angleX = input->getMouseXAxis() / 2.0;
			float angleY = input->getMouseYAxis() / 2.0;

			camera->transform()->rotateAround(target, Transform::up(), -angleX);
			camera->transform()->rotateAround(target, camera->transform()->getRight(), -angleY);
		}

		if (input->getMouseWheelAxis() < 0 || input->getMouseWheelAxis() != 0 && camera->transform()->getPosition().distanceToPoint(target) >= 1)
		{
			float mouseWheelDelta = input->getMouseWheelAxis() / 100.0;
			auto cameraComponent = camera->getComponent<Camera>();
			if (cameraComponent->isOrthographic())
				cameraComponent->setSize(cameraComponent->getSize() - mouseWheelDelta);
			camera->transform()->setPosition(camera->transform()->getPosition() + camera->transform()->getForward() * mouseWheelDelta);
		}

		if ((input->getMouseButtons() & Qt::MiddleButton) == Qt::MiddleButton)
		{
			float deltaX = input->getMouseXAxis() / 20.0;
			float deltaY = input->getMouseYAxis() / 20.0;

			camera->transform()->setPosition(camera->transform()->getPosition() - camera->transform()->getUp() * deltaY);
			camera->transform()->setPosition(camera->transform()->getPosition() + camera->transform()->getRight() * deltaX);

			target = target - camera->transform()->getUp() * deltaY;
			target = target + camera->transform()->getRight() * deltaX;
		}

		camera->transform()->lookAt(target, camera->transform()->getUp());
	}
};
