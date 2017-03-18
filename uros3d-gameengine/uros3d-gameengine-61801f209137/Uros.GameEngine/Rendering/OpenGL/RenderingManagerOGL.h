#pragma once
#include <QOpenGLShaderProgram>
#include "IncludesGL.h"
#include "Rendering/RenderBuffer.h"
#include "Rendering/RenderingManager.h"

namespace GameEngine {

	class RenderingManagerOGL : public RenderingManagerInstance, protected OpenGLFuncs
	{
		NOCOPY(RenderingManagerOGL)
		QOpenGLShaderProgram _shader;
		QOpenGLShaderProgram _skyBoxShader;
		QHash<QString, GLuint> _textureMap;
		QHash<const GeometryBase*, GLuint> _vboMap;
		QHash<const SkyBox*, GLuint> _skyBoxMap;
	public:
		RenderingManagerOGL();
		~RenderingManagerOGL() override;
		void initialize() override;
		RenderBuffer* createRenderBuffer(int width, int height, RenderBufferFormat format) override;
		void setActiveCamera(const Camera* camera) override;
		void setActiveLights(const QList<Light*>& lights) override;
		void pushTransform(const QMatrix4x4& transform) override;
		void popTransform(QMatrix4x4* outTransform) override;
		void bindMaterial(const Material& material) override;
		void draw(const GeometryBase* geometry) override;
		void draw(const SkyBox* skyBox) override;
		void dbgDrawLines(const Segment3D* segments, int count, const QColor& color, float thickness = 1.0f) override;
	private:
		GLuint getVBO(const GeometryBase* geometry);
	};
}
