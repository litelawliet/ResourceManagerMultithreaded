#pragma once

#include <vector>

#include <Rendering/Export.h>

#include <Rendering/Buffers/VertexBuffer.h>
#include <Rendering/Buffers/VertexArray.h>
#include <Rendering/Buffers/IndexBuffer.h>

#include <Rendering/Geometry/Vertex.h>

#include <Rendering/Resources/Shader.h>
#include <Rendering/Resources/Texture.h>
#include <Rendering/Resources/Loaders/MeshLoader.h>
#include <string>

namespace Rendering::Resources
{
	class API_RENDERING Mesh
	{
	public:
		explicit Mesh(const std::string& p_filePath);
		Mesh(Loaders::MeshLoader& p_meshLoader) noexcept;
		Mesh(const Mesh& p_mesh);
		Mesh(Mesh&& p_mesh) noexcept;
		Mesh(const std::vector<Geometry::Vertex>& p_vertices, const std::vector<GLuint>& p_indices);
		~Mesh() noexcept;

		void AddTexture(const std::string& p_texturePath) const;
		void AddNormalMap(const std::string& p_texturePath) const;

		void Bind() const noexcept;
		void BindTextures() const noexcept;
		void Unbind() const noexcept;
		void UnbindTextures() const noexcept;

		inline bool IsVAOGenerated() const;
		inline size_t GetVertexCount() const noexcept;
		inline size_t GetIndicesCount() const noexcept;
		inline bool IsMeshLoaded() const noexcept;
		inline std::string GetName() const noexcept;
		inline Texture* GetTexture() const { return m_texture; }
		inline Texture* GetNormalMap() const { return m_normalMap; }

		void CreateBuffers(const std::vector<Geometry::Vertex>& p_vertices,
			const std::vector<GLuint>& p_indices) noexcept;
		void CreateBuffers() noexcept;
		void SetName(const std::string& p_name);

		Mesh& operator=(const Mesh& p_mesh);
		void GetLoaderData(const Loaders::MeshLoader& p_loader);

	private:
		std::vector<Geometry::Vertex> m_vertices;
		std::vector<uint32_t> m_indices;
		std::string m_name;

		bool m_meshLoaded{ false };
		
		size_t m_vertexCount{};
		size_t m_indicesCount{};

		Buffers::VertexArray* m_vertexArray{ nullptr };
		Buffers::VertexBuffer* m_vboPosition{ nullptr };
		Buffers::VertexBuffer* m_vboTextCoords{ nullptr };
		Buffers::VertexBuffer* m_vboNormal{ nullptr };
		Buffers::VertexBuffer* m_vboTangent{ nullptr };
		Buffers::VertexBuffer* m_vboBitangent{ nullptr };
		Buffers::IndexBuffer* m_indexBuffer{ nullptr };
		Texture* m_texture{ nullptr };
		Texture* m_normalMap{ nullptr };
	};
}
