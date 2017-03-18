#pragma once 
using namespace GameEngine;

class SunGlow final : public Behaviour
{
public:
	explicit SunGlow(GameObject* gameObject)
		: Behaviour(gameObject),
		  camera(nullptr) { }

	void update(double deltaTime) override
	{
		if (camera)
		{
			auto dir = camera->gameObject()->transform()->getPosition() - gameObject()->transform()->getPosition();
			dir.normalize();
			gameObject()->transform()->setUp(dir);
		}
	}

	Camera* camera;
};
