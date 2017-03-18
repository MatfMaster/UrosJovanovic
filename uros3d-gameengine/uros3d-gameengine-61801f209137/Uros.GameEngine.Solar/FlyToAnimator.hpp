#pragma once
#include "Behaviour.h"

using namespace GameEngine;

class FlyToAnimator : public Behaviour
{
public:
	Transform* target;
	Transform* lookAt;
	QVector3D upVector;
	float speed;

	explicit FlyToAnimator(GameObject* gameObject)
		: Behaviour(gameObject),
		  target(nullptr),
		  lookAt(nullptr),
		  speed(10) {}

	void update(double deltaTime) override
	{
		float distance = 0;
		if (auto renderer = target->gameObject()->getComponent<Renderer>())
		{
			auto camera = gameObject()->getComponent<Camera>();
			float vFov = camera->getFOV();
			float hFov = vFov * 1 / camera->getAspectRatio();
			float height = renderer->boundingBox().extent().y() / 2.0f;
			distance = height / tanf(fminf(vFov, hFov) / 2.0f * 3.14 / 180.0);
		}

		auto startPos = gameObject()->transform()->getPosition();
		auto startRot = gameObject()->transform()->getRotation();
		auto targetPos = target->getPosition();
		auto targetRot = QQuaternion::fromDirection((lookAt->getPosition() - startPos).normalized(), upVector);

		auto movement = targetPos - startPos;
		targetPos = targetPos - movement.normalized() * distance;
		movement = targetPos - startPos;
		float t = fmin(1, fmax(0, deltaTime * speed));
		gameObject()->transform()->setPosition(startPos + movement * t);
		gameObject()->transform()->setRotation(QQuaternion::slerp(startRot, targetRot, deltaTime * speed));
	}
};
