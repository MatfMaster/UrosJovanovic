#pragma once 
#include <QElapsedTimer>
#include "IO/InputManager.h"
#include "Geometry/Ray3D.h"
#include "Scene/Raycast.h"

using namespace GameEngine;

class CameraController : public Behaviour
{
public:
	explicit CameraController(GameObject* gameObject)
		: Behaviour(gameObject),
		  speed(100) { }

	void update(double deltaTime) override
	{
		auto camera = gameObject()->getComponent<Camera>();
		auto animator = gameObject()->getComponent<FlyToAnimator>();
		auto scene = ProjectManager::instance()->activeProject()->getActiveScene();
		auto input = InputManager::instance();

		if (input->getMouseButtonDown() & Qt::LeftButton)
		{
			animator->disable();

			float angleX = input->getMouseXAxis() * 100 * deltaTime;
			float angleY = input->getMouseYAxis() * 100 * deltaTime;

			gameObject()->transform()->rotateAround(gameObject()->transform()->getPosition(), Transform::up(), -angleX);
			gameObject()->transform()->rotateAround(gameObject()->transform()->getPosition(), gameObject()->transform()->getRight(), -angleY);
		}

		if (input->getMouseButtonUp() & Qt::LeftButton)
		{
			if (doubleClickTimer.elapsed() <= 200)
			{
				// Left mouse double click

				float mouseX = input->getMouseX();
				float mouseY = input->getMouseY();

				QVector3D near = camera->screenToWorld(QVector3D(mouseX, mouseY, 0));
				QVector3D far = camera->screenToWorld(QVector3D(mouseX, mouseY, 1));
				Ray3D ray = Ray3D(near, (far - near).normalized());

				GameObject* hitObject = Raycast(ray).gameObject();
				if (hitObject && hitObject->getName() != "Blue Orb")
				{
					DEBUG_LOG(hitObject->getName().toStdString());
					animator->target = hitObject->transform();
					animator->lookAt = hitObject->transform();
					animator->upVector = Transform::up();
					animator->enable();
				}
			}
			doubleClickTimer.restart();
		}

		if (input->getMouseWheelAxis() != 0)
		{
			float mouseWheelDelta = input->getMouseWheelAxis();
			speed += (signbit(mouseWheelDelta) ? -1 : 1) * 1000 * deltaTime;
			speed = fmax(speed, 10);
			DEBUG_LOG(speed);
		}

		if ((input->getMouseButtons() & Qt::MiddleButton) == Qt::MiddleButton)
		{
			animator->disable();

			float deltaX = input->getMouseXAxis() * 10 * deltaTime;
			float deltaY = input->getMouseYAxis() * 10 * deltaTime;

			gameObject()->transform()->setPosition(gameObject()->transform()->getPosition() - gameObject()->transform()->getUp() * deltaY);
			gameObject()->transform()->setPosition(gameObject()->transform()->getPosition() + gameObject()->transform()->getRight() * deltaX);
		}

		int vertical = 0;
		int horizontal = 0;

		if (input->keyPressed(Qt::Key_W))
			vertical = 1;
		if (input->keyPressed(Qt::Key_S))
			vertical = -1;
		if (input->keyPressed(Qt::Key_A))
			horizontal = 1;
		if (input->keyPressed(Qt::Key_D))
			horizontal = -1;

		if (vertical != 0 || horizontal != 0)
			animator->disable();

		if (input->keyPressed(Qt::Key_Space))
		{
			auto vantage = scene->findGameObject("VantagePoint");
			auto sun = scene->findGameObject("Sun");
			if (vantage)
			{
				animator->target = vantage->transform();
				animator->lookAt = sun->transform();
				animator->upVector = QVector3D(0, 0, 1);
				animator->enable();
			}
		}

		GameObject* target = nullptr;
		switch (input->getKey())
		{
			case Qt::Key_1:
				target = scene->findGameObject("Mercury");
				break;
			case Qt::Key_2:
				target = scene->findGameObject("Venus");
				break;
			case Qt::Key_3:
				target = scene->findGameObject("Earth");
				break;
			case Qt::Key_4:
				target = scene->findGameObject("Mars");
				break;
			case Qt::Key_5:
				target = scene->findGameObject("Jupiter");
				break;
			case Qt::Key_6:
				target = scene->findGameObject("Saturn");
				break;
			case Qt::Key_7:
				target = scene->findGameObject("Uranus");
				break;
			case Qt::Key_8:
				target = scene->findGameObject("Neptune");
				break;
			default:
				break;
		}

		if (target)
		{
			animator->target = target->transform();
			animator->lookAt = target->transform();
			animator->upVector = Transform::up();
			animator->enable();
		}

		gameObject()->transform()->translate(vertical * gameObject()->transform()->getForward() * speed * deltaTime);
		gameObject()->transform()->translate(horizontal * gameObject()->transform()->getRight() * speed * deltaTime);
	}

private:
	float speed;
	QElapsedTimer doubleClickTimer;
};
