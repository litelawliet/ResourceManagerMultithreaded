#pragma once
#include <Rendering/Export.h>
#include <future>
#include <Rendering/Geometry/Vertex.h>
#include <string_view>

namespace Rendering::Resources::Loaders
{
	class API_RENDERING MeshLoader
	{
	public:
		MeshLoader() = delete;
		~MeshLoader();

		explicit MeshLoader(const std::string_view& p_filePath);

		inline size_t GetVerticesCount() const;
		inline size_t GetIndicesCount() const;
		inline std::vector<Geometry::Vertex> GetVertices() const;
		inline std::vector<GLuint> GetIndices() const;

	private:
		static void ReadFile(const std::string_view& p_filePath,std::string&& p_fileContent);
		void ProcessMesh(std::vector<Geometry::Vertex>& p_vertices, std::vector<GLuint>& p_indices,
		                        const std::string_view& p_fileContent);

		static void ParseVec3(const std::string_view& p_lineToParse, const std::string_view& p_headerLine, std::vector<glm::vec3>& p_array);
		static void ParseVec2(const std::string_view& p_lineToParse, const std::string_view& p_headerLine, std::vector<glm::vec2>& p_array);
		void CheckIndices(const std::string_view& p_str);
		void AddIndices(const std::string_view& p_str, std::vector<GLuint>& p_vertexIndices, std::vector<GLuint>& p_uvIndices, std::vector<GLuint>& p_normalIndices);

		std::vector<Geometry::Vertex> m_vertices;
		std::vector<GLuint> m_indices;
		std::string m_fileContent;
		bool m_hasUV;
		bool m_hasNormals;
		bool m_isQuads;
		bool m_indexChecked;
	};
}
