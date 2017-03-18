#pragma once
#include "GeometryBase.h"

namespace GameEngine {
	class RenderingManagerOGL;
	class Mesh final : public GeometryBase
	{
		NOCOPY(Mesh)

	private:
		Mesh();
		float* _vertices;
		float* _normals;
		float* _texcoords;
		int _verticesCount;
		BoundingBox _boundingBox;

	public:
		Mesh(float* vertices, float* normals, int count, float* texcoords = nullptr);
		Mesh(float* vertices, float* normals, float* texcoords, int count);
		~Mesh() override;

		static Mesh* cone();
		static Mesh* cube();
		static Mesh* plane();
		static Mesh* sphere();
		static Mesh* cylinder();

		/* GeometryBase Members */

		GeometryBase* clone() const override;
		int vertexCount() const override;
		int triangleCount() const override;
		void getVertexData(int index, QVector3D& v, QVector3D& n) const override;
		void getTriangleData(int index, QVector3D& t0, QVector3D& t1, QVector3D& t2, QVector3D& n0, QVector3D& n1, QVector3D& n2) const override;
		const BoundingBox& boundingBox() const override;

		/* Members */

		void getTextureCoord(int index, QVector3D& coord);

		/* Friend classes */

		friend class RenderingManagerOGL;
	};
}
