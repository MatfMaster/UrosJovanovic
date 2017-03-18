#pragma once
#include "Includes.h"
#include "IncludesGL.h"
#include "Rendering/Viewport.h"

namespace GameEngine {
	class ViewportGL final : public Viewport, OpenGLFuncs
	{
		Q_OBJECT

		NOCOPY(ViewportGL)

		QOpenGLContext* _context;

	public:
		explicit ViewportGL(QWindow* parent = nullptr);
		~ViewportGL() override;

	protected:

		/* Viewport overrides */

		void init() override;
		void onBeforeRender() override;
		void onAfterRender() override;
		void onAfterResize(int width, int height) override;
	};
}
