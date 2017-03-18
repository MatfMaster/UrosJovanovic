#pragma once 
#include "BoundingBox.h"

namespace GameEngine {
	class GeometryBase
	{
	protected:
		GeometryBase() {};

	public:
		virtual ~GeometryBase() {};

		EXPORT virtual GeometryBase* clone() const = 0;
		EXPORT virtual int vertexCount() const = 0;
		EXPORT virtual int triangleCount() const = 0;
		EXPORT virtual void getVertexData(int index, QVector3D& v, QVector3D& n) const = 0;
		EXPORT virtual void getTriangleData(int index, QVector3D& t0, QVector3D& t1, QVector3D& t2, QVector3D& n0, QVector3D& n1, QVector3D& n2) const = 0;
		EXPORT virtual const BoundingBox& boundingBox() const = 0;
	};
}
