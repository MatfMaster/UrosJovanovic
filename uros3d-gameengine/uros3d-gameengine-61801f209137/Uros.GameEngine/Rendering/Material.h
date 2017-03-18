#pragma once
#include <QColor>
#include "Texture.h"

namespace GameEngine {
	class Material final
	{
	public:
		enum ShaderType
		{
			Unlit = 0,
			Standard = 1,
			UnlitTransparent = 100,
			StandardTransparent = 101
		};

		EXPORT Material();
		EXPORT Material(const QColor& diffuseColor);

		EXPORT const ShaderType& getShaderType() const;
		EXPORT const QColor& getDiffuseColor() const;
		EXPORT const QColor& getSpecularColor() const;
		EXPORT float getShininess() const;
		EXPORT float getOpacity() const;
		EXPORT const Texture& getConstTexture() const;
		EXPORT Texture& getTexture();
		EXPORT bool isTwoSided() const;

		EXPORT void setShaderType(const ShaderType& shaderType);
		EXPORT void setDiffuseColor(const QColor& color);
		EXPORT void setSpecularColor(const QColor& color);
		EXPORT void setShininess(float amount);
		EXPORT void setOpacity(float opacity);
		EXPORT void setTexture(const Texture& texture);
		EXPORT void enableTwoSided();
		EXPORT void disableTwoSided();

		EXPORT bool operator <(const Material& other) const;
		EXPORT bool operator >(const Material& other) const;
		EXPORT bool operator <=(const Material& other) const;
		EXPORT bool operator >=(const Material& other) const;
		EXPORT bool operator ==(const Material& other) const;
		EXPORT bool operator !=(const Material& other) const;

	private:
		ShaderType _shaderType;
		QColor _diffuseColor;
		QColor _specularColor;
		float _shininess;
		float _opacity;
		Texture _texture;
		bool _twoSided;
	};

	uint qHash(const Material& material);
}
