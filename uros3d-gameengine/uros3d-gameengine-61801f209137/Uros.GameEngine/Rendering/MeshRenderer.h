#pragma once
#include "Renderer.h"
#include "Geometry/Mesh.h"

namespace GameEngine {
	class MeshRenderer final : public Renderer
	{
		NOCOPY(MeshRenderer)

	public:
		explicit MeshRenderer(GameObject* gameObject);

	protected:
		~MeshRenderer() override;

	public:
		EXPORT const BoundingBox& boundingBox() override;
		EXPORT Mesh* getMesh() const;
		EXPORT void setMesh(Mesh* mesh);
		void render() override;

	protected slots:
		void onTransformChanged(Transform* transform) override;

	private:
		Mesh* _mesh;
		BoundingBox* _bbox;
		int _frameID;
	};
}
