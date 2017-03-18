#include "Light.h"
#include "GameObject.h"

QColor GameEngine::Light::_ambientColor = QColor();

GameEngine::Light::Light(GameObject* gameObject)
	: Component(gameObject),
	  _enabled(true),
	  _type(Point),
	  _color(Qt::white),
	  _intensity(1),
	  _range(10),
	  _spotOuterAngle(15),
	  _spotInnerAngle(10) {}

GameEngine::Component::ComponentType GameEngine::Light::type() const
{
	return T_LIGHT;
}

bool GameEngine::Light::isSingular() const
{
	return true;
}

GameEngine::Light::~Light() {}

GameEngine::GameObject* GameEngine::Light::create(const LightType& type)
{
	auto gameObject = new GameObject("Light");
	auto light = gameObject->addComponent<Light>();
	light->setType(type);
	if (type == Directional)
		light->setIntensity(0.5);
	return gameObject;
}

void GameEngine::Light::setAmbientLightColor(const QColor& color)
{
	_ambientColor = color;
}

const QColor& GameEngine::Light::getAmbientLightColor()
{
	return _ambientColor;
}

bool GameEngine::Light::isEnabled() const
{
	return _enabled;
}

const GameEngine::Light::LightType& GameEngine::Light::getType() const
{
	return _type;
}

const QColor& GameEngine::Light::getColor() const
{
	return _color;
}

float GameEngine::Light::getIntensity() const
{
	return _intensity;
}

float GameEngine::Light::getRange() const
{
	return _range;
}

float GameEngine::Light::getSpotOuterAngle() const
{
	return _spotOuterAngle;
}

float GameEngine::Light::getSpotInnerAngle() const
{
	return _spotInnerAngle;
}

void GameEngine::Light::enable()
{
	_enabled = true;
}

void GameEngine::Light::disable()
{
	_enabled = false;
}

void GameEngine::Light::setType(const LightType& type)
{
	_type = type;
}

void GameEngine::Light::setColor(const QColor& color)
{
	_color = color;
}

void GameEngine::Light::setIntensity(float intensity)
{
	if (intensity < 0)
		intensity = 0;
	_intensity = intensity;
}

void GameEngine::Light::setRange(float radius)
{
	if (radius < 0)
		radius = 0;
	_range = radius;
}

void GameEngine::Light::setSpotOuterAngle(float degrees)
{
	if (degrees < 0)
		degrees = 0;
	if (degrees > 90)
		degrees = 90;
	_spotOuterAngle = degrees;
}

void GameEngine::Light::setSpotInnerAngle(float degrees)
{
	if (degrees < 0)
		degrees = 0;
	if (degrees > 90)
		degrees = 90;
	_spotInnerAngle = degrees;
}
