#pragma once
#include "Behaviour.h"
#include "GameObject.h"

class RotateAround final : public GameEngine::Behaviour
{
public:
	explicit RotateAround(GameEngine::GameObject* gameObject);

protected:
	~RotateAround() override;

public:
	void update(double deltaTime) override;
};
