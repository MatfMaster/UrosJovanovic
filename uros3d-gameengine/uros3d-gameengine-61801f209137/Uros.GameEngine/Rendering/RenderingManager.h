#pragma once
#include <QMatrix4x4>
#include <QElapsedTimer>
#include "MeshRenderer.h"
#include "RenderBuffer.h"
#include "RendererBatch.h"
#include "RenderStats.h"
#include "Geometry/Segment3D.h"

namespace GameEngine {
	class SkyBox;
	class Camera;
	class Light;
	class Material;
	class GeometryBase;

	class RenderingManagerInstance
	{
	protected:
		RenderingManagerInstance();

	public:
		virtual ~RenderingManagerInstance();

		virtual void initialize() = 0;
		virtual RenderBuffer* createRenderBuffer(int width, int height, RenderBufferFormat format) = 0;
		virtual void setActiveCamera(const Camera* camera) = 0;
		virtual void setActiveLights(const QList<Light*>& lights) = 0;
		virtual void pushTransform(const QMatrix4x4& transform) = 0;
		virtual void popTransform(QMatrix4x4* outTransform = nullptr) = 0;
		virtual void bindMaterial(const Material& material) = 0;
		virtual void draw(const GeometryBase* geometry) = 0;
		virtual void draw(const SkyBox*  skyBox) = 0;
		virtual void dbgDrawLines(const Segment3D* segments, int count, const QColor& color, float thickness = 1.0f) = 0;

		template<class MeshRendererItor>
		void buildStaticBatches(MeshRendererItor begin, MeshRendererItor end)
		{
			QElapsedTimer timer;
			timer.start();
			auto itor = begin;
			while (itor != end)
			{
				const MeshRenderer* renderer = *itor;
				const Material& material = renderer->getConstMaterial();
				auto it = _staticBatches.constFind(material);
				if (it != _staticBatches.constEnd())
					(*it)->insert(renderer);
				else
					_staticBatches.insert(material, new RendererBatch<MeshRenderer>(material, renderer));
				++itor;
			}
			int size = 0;
			for (auto batch : _staticBatches)
			{
				batch->build();
				size += batch->geometrySize();
			}
			_stats.setBatchCount(_staticBatches.count());
			_stats.setBatchSize(size);
			DEBUG_LOG("> RenderingManager::buildStaticBatches: took " << timer.elapsed() / 1000.0 << "s");
		}
		void drawOpaqueBatches(const Camera* activeCamera = nullptr);
		void drawTransparentBatches(const Camera* activeCamera = nullptr);
		RenderStats& stats();

	private:
		RenderStats _stats;
		QHash<Material, RendererBatch<MeshRenderer>*> _staticBatches;
	};

	class RenderingManager final
	{
		static RenderingManagerInstance* _instance;

	public:
		static RenderingManagerInstance* instance();
		static void initialize(RenderingManagerInstance* instance);
		static void destroy();
	};
}
