#pragma once
#include "Includes.h"

namespace GameEngine {
	class Settings final
	{
	public:
		enum WindowType
		{
			Window,
			WindowBorderless,
			Fullscreen
		};
		enum AntiAliasingType
		{
			Off,
			MSAAx4,
			MSAAx8,
			MSAAx16
		};
		enum RendererType
		{
			Automatic,
			OpenGL,
			Direct3D
		};

		EXPORT Settings();

		EXPORT bool isVSyncEnabled() const;
		EXPORT WindowType getWindowType() const;
		EXPORT AntiAliasingType getAntiAliasingType() const;
		EXPORT RendererType getRendererType() const;

		EXPORT void enableVSync();
		EXPORT void disableVSync();
		EXPORT void setWindowType(const WindowType& type);
		EXPORT void setAntialiasingType(const AntiAliasingType& type);
		EXPORT void setRendererType(const RendererType& type);

	private:
		bool _vSync;
		WindowType _windowType;
		AntiAliasingType _antiAliasingType;
		RendererType _rendererType;
	};
}
