#pragma once
using namespace GameEngine;

class RotateAround : public Behaviour
{
public:
	explicit RotateAround(GameObject* gameObject)
		: Behaviour(gameObject),
		  speed(10),
		  target(nullptr),
		  selfRotationSpeed(0) { }

	void update(double deltaTime) override
	{
		if (target)
			gameObject()->transform()->rotateAround(target->getPosition(), Transform::up(), speed * deltaTime);
		gameObject()->transform()->rotate(Transform::up(), selfRotationSpeed * deltaTime);
	}

	float speed;
	Transform* target;
	float selfRotationSpeed;
};
