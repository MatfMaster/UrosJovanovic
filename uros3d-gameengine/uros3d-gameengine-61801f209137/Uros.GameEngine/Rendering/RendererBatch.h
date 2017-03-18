#pragma once
#include "Renderer.h"
#include "MeshRenderer.h"
#include "Geometry/GeometryBase.h"

namespace GameEngine {
	template <typename RendererType>
	class RendererBatch final
	{
	private:
		Material _material;
		QSet<const RendererType*> _renderers;
		GeometryBase* _geometry;

	public:
		RendererBatch(const Material& material, const RendererType* renderer);
		RendererBatch(const Material& material, const RendererType* renderers, int count);
		template <class RendererItor>
		RendererBatch(const Material& material, RendererItor begin, RendererItor end);
		~RendererBatch();

		const Material& material() const;
		const QSet<const RendererType*>& renderers() const;
		const GeometryBase* geometry() const;
		int geometrySize() const;

		bool insert(const RendererType* renderer);
		bool insert(const RendererType* renderers, int count);
		template <class RendererItor>
		bool insert(RendererItor begin, RendererItor end);

		bool remove(const RendererType* renderer);
		bool remove(const RendererType* renderers, int count);
		template <class RendererItor>
		bool remove(RendererItor begin, RendererItor end);

		void build();
	};

	template <typename RendererType>
	RendererBatch<RendererType>::RendererBatch(const Material& material, const RendererType* renderer)
	{
		static_assert(IS_OF_TYPE(RendererType, Renderer), "Type must be derived from Renderer class");

		_material = material;
		_geometry = nullptr;
		_renderers.insert(renderer);
	}

	template <typename RendererType>
	RendererBatch<RendererType>::RendererBatch(const Material& material, const RendererType* renderers, int count)
	{
		static_assert(IS_OF_TYPE(RendererType, Renderer), "Type must be derived from Renderer class");

		_material = material;
		_geometry = nullptr;
		for (int i = 0; i < count; i++)
			_renderers.insert(renderers + i);
	}

	template <typename RendererType>
	template <class RendererItor>
	RendererBatch<RendererType>::RendererBatch(const Material& material, RendererItor begin, RendererItor end)
	{
		static_assert(IS_OF_TYPE(RendererType, Renderer), "Type must be derived from Renderer class");

		_material = material;
		_geometry = nullptr;
		auto itor = begin;
		while (itor != end)
		{
			_renderers.insert(*itor);
			++itor;
		}
	}

	template <typename RendererType>
	RendererBatch<RendererType>::~RendererBatch()
	{
		delete _geometry;
	}

	template <typename RendererType>
	const Material& RendererBatch<RendererType>::material() const
	{
		return _material;
	}

	template <typename RendererType>
	const QSet<const RendererType*>& RendererBatch<RendererType>::renderers() const
	{
		return _renderers;
	}

	template <typename RendererType>
	const GeometryBase* RendererBatch<RendererType>::geometry() const
	{
		return _geometry;
	}

	template <typename RendererType>
	int RendererBatch<RendererType>::geometrySize() const
	{
		return _geometry ? _geometry->vertexCount() * 3 * sizeof(float) * 3 : 0;
	}

	template <typename RendererType>
	bool RendererBatch<RendererType>::insert(const RendererType* renderer)
	{
		return _renderers.insert(renderer) != _renderers.end();
	}

	template <typename RendererType>
	bool RendererBatch<RendererType>::insert(const RendererType* renderers, int count)
	{
		bool result = false;
		for (int i = 0; i < count; i++)
			result |= insert(renderers + i);
		return result;
	}

	template <typename RendererType>
	template <class RendererItor>
	bool RendererBatch<RendererType>::insert(RendererItor begin, RendererItor end)
	{
		bool result = false;
		auto itor = begin;
		while (itor != end)
		{
			result |= insert(*itor);
			++itor;
		}
		return result;
	}

	template <typename RendererType>
	bool RendererBatch<RendererType>::remove(const RendererType* renderer)
	{
		return _renderers.remove(renderer);
	}

	template <typename RendererType>
	bool RendererBatch<RendererType>::remove(const RendererType* renderers, int count)
	{
		bool result = false;
		for (int i = 0; i < count; i++)
			result |= remove(renderers + i);
		return result;
	}

	template <typename RendererType>
	template <class RendererItor>
	bool RendererBatch<RendererType>::remove(RendererItor begin, RendererItor end)
	{
		bool result = false;
		auto itor = begin;
		while (itor != end)
		{
			result |= remove(*itor);
			++itor;
		}
		return result;
	}

	template <typename RendererType>
	void RendererBatch<RendererType>::build()
	{
		if (IS_OF_TYPE(RendererType, MeshRenderer))
		{
			if (_geometry)
				delete _geometry;

			int count = 0;
			for (auto renderer : _renderers)
				if (auto geometry = renderer->getMesh())
					count += geometry->vertexCount() * 3;

			std::vector<float> vertices(count);
			std::vector<float> normals(count);
			std::vector<float> texcoords(count);
			int vPos = 0;
			int nPos = 0;
			int cPos = 0;
			for (auto renderer : _renderers)
			{
				if (Mesh* geometry = renderer->getMesh())
				{
					QVector3D v, n, c;
					const QMatrix4x4& transform = renderer->gameObject()->transform()->getMatrix();
					for (int i = 0; i < geometry->vertexCount(); i++)
					{
						geometry->getVertexData(i, v, n);
						geometry->getTextureCoord(i, c);
						v = transform * v;
						vertices[vPos++] = v[0];
						vertices[vPos++] = v[1];
						vertices[vPos++] = v[2];
						normals[nPos++] = n[0];
						normals[nPos++] = n[1];
						normals[nPos++] = n[2];
						texcoords[cPos++] = c[0];
						texcoords[cPos++] = c[1];
						texcoords[cPos++] = c[2];
					}
				}
			}
			_geometry = new Mesh(vertices.data(), normals.data(), texcoords.data(), count);
		}
		else
		ERROR_LOG("> RendererBatch::build() Unsupported renderer type.");
	}
}
