#pragma once
#include <QColor>
#include "Component.h"

namespace GameEngine {
	class Light final : public Component
	{
		NOCOPY(Light)

	public:
		enum LightType
		{
			Spot = 0,
			Point = 1,
			Directional = 2
		};

	private:
		static QColor _ambientColor;
		bool _enabled;
		LightType _type;
		QColor _color;
		float _intensity;
		float _range;
		float _spotOuterAngle;
		float _spotInnerAngle;

	protected:
		~Light() override;

	public:
		explicit Light(GameObject* gameObject);

		EXPORT ComponentType type() const override;
		EXPORT bool isSingular() const override;

		EXPORT static GameObject* create(const LightType& type);
		EXPORT static const QColor& getAmbientLightColor();
		EXPORT static void setAmbientLightColor(const QColor& color);

		EXPORT bool isEnabled() const;
		EXPORT const LightType& getType() const;
		EXPORT const QColor& getColor() const;
		EXPORT float getIntensity() const;
		EXPORT float getRange() const;
		EXPORT float getSpotOuterAngle() const;
		EXPORT float getSpotInnerAngle() const;

		EXPORT void enable();
		EXPORT void disable();
		EXPORT void setType(const LightType& type);
		EXPORT void setColor(const QColor& color);
		EXPORT void setIntensity(float intensity);
		EXPORT void setRange(float radius);
		EXPORT void setSpotOuterAngle(float degrees);
		EXPORT void setSpotInnerAngle(float degrees);
	};
}
