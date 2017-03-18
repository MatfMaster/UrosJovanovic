#pragma once 
#include <QVector3D>
#include "Behaviour.h"

#define Input (*GameEngine::InputManager::instance())

class CameraController : public GameEngine::Behaviour
{
	QVector3D target;
public:
	explicit CameraController(GameEngine::GameObject* gameObject);

	void update(double deltaTime) override;

	const QVector3D& getTarget() const;
	void setTarget(const QVector3D& target);
};
