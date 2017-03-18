#include "Transform.h"
#include "Behaviour.h"
#include "GameObject.h"

using namespace GameEngine;

class RotateAround final : public Behaviour
{
protected:
	~RotateAround() override {};

public:
	explicit RotateAround(GameObject* gameObject)
		: Behaviour(gameObject) {}

	void update(double deltaTime) override
	{
		gameObject()->transform()->rotate(Transform::up(), 1);
	}
};
