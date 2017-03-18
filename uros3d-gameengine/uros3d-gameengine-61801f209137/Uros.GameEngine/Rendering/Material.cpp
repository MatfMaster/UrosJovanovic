#include <QHash>
#include "Material.h"

GameEngine::Material::Material()
	: _shaderType(Standard),
	  _diffuseColor(Qt::white),
	  _specularColor(Qt::black),
	  _shininess(0),
	  _opacity(1),
	  _twoSided(false) {}

GameEngine::Material::Material(const QColor& diffuseColor)
	: Material()
{
	_diffuseColor = diffuseColor;
}

const QColor& GameEngine::Material::getDiffuseColor() const
{
	return _diffuseColor;
}

const QColor& GameEngine::Material::getSpecularColor() const
{
	return _specularColor;
}

float GameEngine::Material::getShininess() const
{
	return _shininess;
}

float GameEngine::Material::getOpacity() const
{
	return _opacity;
}

const Texture& GameEngine::Material::getConstTexture() const
{
	return _texture;
}

Texture& GameEngine::Material::getTexture()
{
	return _texture;
}

bool GameEngine::Material::isTwoSided() const
{
	return _twoSided;
}

const GameEngine::Material::ShaderType& GameEngine::Material::getShaderType() const
{
	return _shaderType;
}

void GameEngine::Material::setDiffuseColor(const QColor& color)
{
	_diffuseColor = color;
}

void GameEngine::Material::setSpecularColor(const QColor& color)
{
	_specularColor = color;
}

void GameEngine::Material::setShininess(float amount)
{
	_shininess = amount < 0 ? 0 : amount;
}

void GameEngine::Material::setOpacity(float opacity)
{
	opacity = fminf(1, fmaxf(0, opacity));
	_opacity = opacity;
}

void GameEngine::Material::setTexture(const Texture& path)
{
	_texture = path;
}

void GameEngine::Material::enableTwoSided()
{
	_twoSided = true;
}

void GameEngine::Material::disableTwoSided()
{
	_twoSided = false;
}

void GameEngine::Material::setShaderType(const ShaderType& shaderType)
{
	_shaderType = shaderType;
}

bool GameEngine::Material::operator<(const Material& other) const
{
	return _shaderType < other._shaderType;
}

bool GameEngine::Material::operator>(const Material& other) const
{
	return other._shaderType < _shaderType;
}

bool GameEngine::Material::operator<=(const Material& other) const
{
	return !(*this > other);
}

bool GameEngine::Material::operator>=(const Material& other) const
{
	return !(*this < other);
}

bool GameEngine::Material::operator==(const Material& other) const
{
	return _shaderType == other._shaderType &&
		_diffuseColor == other._diffuseColor &&
		_specularColor == other._specularColor &&
		_shininess == other._shininess &&
		_texture == other._texture &&
		_twoSided == other._twoSided;
}

bool GameEngine::Material::operator!=(const Material& other) const
{
	return !(*this == other);
}

uint GameEngine::qHash(const Material& material)
{
	return ::qHash(material.getShaderType() * 397) ^
		::qHash(material.getDiffuseColor().red()) ^
		::qHash(material.getDiffuseColor().green()) ^
		::qHash(material.getDiffuseColor().blue()) ^
		::qHash(material.getSpecularColor().red()) ^
		::qHash(material.getSpecularColor().green()) ^
		::qHash(material.getSpecularColor().blue()) ^
		::qHash(material.getShininess()) ^
		::qHash(material.getConstTexture().path()) ^
		::qHash(material.isTwoSided());
}
