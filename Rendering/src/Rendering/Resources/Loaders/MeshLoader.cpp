#include <stdafx.h>
#include <Rendering/Resources/Loaders/MeshLoader.h>
#include <thread>
#include <fstream>
#include <sstream>
#include <future>
#include <algorithm>
#include <chrono>

Rendering::Resources::Loaders::MeshLoader::~MeshLoader()
{
	m_vertices.clear();
	m_indices.clear();
}

Rendering::Resources::Loaders::MeshLoader::MeshLoader(const std::string_view& p_filePath)
	: m_hasUV{ false }, m_hasNormals{ false }, m_isQuads{ false }, m_indexChecked{ false }
{
	const std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	ReadFile(p_filePath.data(), std::move(m_fileContent));

	std::thread processThread(&MeshLoader::ProcessMesh, this, std::ref(m_vertices),
		std::ref(m_indices),
		m_fileContent.c_str());
	//ProcessMesh(std::ref(m_vertices), std::ref(m_indices), std::ref(m_fileContent));
	processThread.join();
	const std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << p_filePath << " - loading time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";
}

inline size_t Rendering::Resources::Loaders::MeshLoader::GetVerticesCount() const
{
	return m_vertices.size();
}

inline size_t Rendering::Resources::Loaders::MeshLoader::GetIndicesCount() const
{
	return m_indices.size();
}

inline std::vector<Rendering::Geometry::Vertex> Rendering::Resources::Loaders::MeshLoader::
GetVertices() const
{
	return m_vertices;
}

inline std::vector<GLuint> Rendering::Resources::Loaders::MeshLoader::GetIndices() const
{
	return m_indices;
}

void Rendering::Resources::Loaders::MeshLoader::ReadFile(
	const std::string_view& p_filePath, std::string&& p_fileContent)
{
	std::ifstream file(p_filePath.data());

	if (file)
	{
		std::stringstream buffer;

		buffer << file.rdbuf();

		file.close();

		p_fileContent = buffer.str();
	}
	else
		p_fileContent = "";
}

void Rendering::Resources::Loaders::MeshLoader::ProcessMesh(
	std::vector<Geometry::Vertex>& p_vertices, std::vector<GLuint>& p_indices,
	const std::string_view& p_fileContent)
{
	std::stringstream parser{ p_fileContent.data() };
	std::string line;
	std::vector<GLuint> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempUvs;
	std::vector<glm::vec3> tempNormals;

	while (std::getline(parser, line))
	{
		if (line.find("v ") == 0)
		{
			ParseVec3(line, "v", std::ref(tempVertices));
		}
		else if (line.find("vt ") == 0)
		{
			ParseVec2(line, "vt", std::ref(tempUvs));
		}
		else if (line.find("vn ") == 0)
		{
			ParseVec3(line, "vn", std::ref(tempNormals));
		}
		else if (line.find("f ") == 0)
		{
			AddIndices(line, std::ref(vertexIndices), std::ref(uvIndices), std::ref(normalIndices));
		}
	}

	for (size_t i = 0; i < tempVertices.size(); ++i)
	{
		glm::vec3 position = tempVertices[i];

		Geometry::Vertex tmpVertex;
		tmpVertex.m_position = position;
		
		if (m_hasUV)
			tmpVertex.m_textCoords = tempUvs[uvIndices[i]];
		else
			tmpVertex.m_textCoords = glm::vec2{};

		if (m_hasNormals)
			tmpVertex.m_normal = tempNormals[normalIndices[i]];
		else
			tmpVertex.m_normal = glm::vec3{};

		tmpVertex.m_biTangent = glm::vec3{ 1.0f };
		tmpVertex.m_tangent = glm::vec3{ 1.0f };

		p_vertices.push_back(tmpVertex);
	}

	for (GLuint& vertexIndex : vertexIndices)
	{
		p_indices.emplace_back(vertexIndex);
	}
}

void Rendering::Resources::Loaders::MeshLoader::ParseVec3(
	const std::string_view& p_lineToParse, const std::string_view& p_headerLine, std::vector<glm::vec3>& p_array)
{
	glm::vec3 vertex;
	const std::string scanStrFormat = p_headerLine.data() + std::string(" %f %f %f");
	sscanf_s(p_lineToParse.data(), scanStrFormat.c_str(), &vertex.x, &vertex.y, &vertex.z);
	p_array.emplace_back(vertex);
}

void Rendering::Resources::Loaders::MeshLoader::ParseVec2(
	const std::string_view& p_lineToParse, const std::string_view& p_headerLine, std::vector<glm::vec2>& p_array)
{
	glm::vec2 vertex;
	const std::string scanStrFormat = p_headerLine.data() + std::string( " %f %f");
	sscanf_s(p_lineToParse.data(), scanStrFormat.c_str(), &vertex.x, &vertex.y);
	p_array.emplace_back(vertex);
}

void Rendering::Resources::Loaders::MeshLoader::CheckIndices(const std::string_view& p_str) {
	if (!m_indexChecked) {
		int bestSample = 0;
		int sampleV1 = -1, sampleU1 = -1, sampleN1 = -1;
		int tmp = -1, sampleV4 = -1;

		const int form1 = sscanf_s(p_str.data(), "f %d", &sampleV1);
		const int form2 = sscanf_s(p_str.data(), "f %d/%d", &sampleV1, &sampleU1);
		const int form3 = sscanf_s(p_str.data(), "f %d//%d", &sampleV1, &sampleN1);
		const int form4 = sscanf_s(p_str.data(), "f %d/%d/%d", &sampleV1, &sampleU1, &sampleN1);

		if (form1 > bestSample) {
			bestSample = form1;
			m_hasUV = false;
			m_hasNormals = false;
		}
		if (form2 > bestSample) {
			bestSample = form2;
			m_hasUV = true;
			m_hasNormals = false;
		}
		if (form3 > bestSample) {
			bestSample = form3;
			m_hasUV = false;
			m_hasNormals = true;
		}
		if (form4 > bestSample) {
			bestSample = form4;
			m_hasUV = true;
			m_hasNormals = true;
		}

		m_indexChecked = true;
	}
}

void Rendering::Resources::Loaders::MeshLoader::AddIndices(const std::string_view& p_str, std::vector<GLuint>& p_vertexIndices, std::vector<GLuint>& p_uvIndices, std::vector<GLuint>& p_normalIndices)
{
	int v1 = -1, u1 = -1, n1 = -1,
		v2 = -1, u2 = -1, n2 = -1,
		v3 = -1, u3 = -1, n3 = -1,
		v4 = -1, u4 = -1, n4 = -1;

	CheckIndices(p_str);


	if (!m_hasUV && !m_hasNormals) {
		sscanf_s(p_str.data(), "f %d %d %d %d",
			&v1,
			&v2,
			&v3,
			&v4);
	}
	else if (m_hasUV && !m_hasNormals) {
		sscanf_s(p_str.data(), "f %d/%d %d/%d %d/%d %d/%d",
			&v1, &u1,
			&v2, &u2,
			&v3, &u3,
			&v4, &u4);
	}
	else if (!m_hasUV && m_hasNormals) {
		sscanf_s(p_str.data(), "f %d//%d %d//%d %d//%d %d//%d",
			&v1, &n1,
			&v2, &n2,
			&v3, &n3,
			&v4, &n4);
	}
	else if (m_hasUV && m_hasNormals) {
		sscanf_s(p_str.data(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
			&v1, &u1, &n1,
			&v2, &u2, &n2,
			&v3, &u3, &n3,
			&v4, &u4, &n4);
	}

	v1--; u1--; n1--;
	v2--; u2--; n2--;
	v3--; u3--; n3--;
	v4--; u4--; n4--;

	if (v3 >= 0)
	{
		p_vertexIndices.push_back(v1);
		p_vertexIndices.push_back(v2);
		p_vertexIndices.push_back(v3);
	}

	if (u3 >= 0)
	{
		p_uvIndices.push_back(u1);
		p_uvIndices.push_back(u2);
		p_uvIndices.push_back(u3);
	}

	if (n3 >= 0)
	{
		p_normalIndices.push_back(n1);
		p_normalIndices.push_back(n2);
		p_normalIndices.push_back(n3);
	}
	
	// in case of quads:
	if (v4 >= 0)
	{
		p_vertexIndices.push_back(v3);
		p_vertexIndices.push_back(v4);
		p_vertexIndices.push_back(v1);
	}
	if (u4 >= 0)
	{
		p_uvIndices.push_back(u3);
		p_uvIndices.push_back(u4);
		p_uvIndices.push_back(u1);
	}
	if (n4 >= 0) 
	{
		p_normalIndices.push_back(n3);
		p_normalIndices.push_back(n4);
		p_normalIndices.push_back(n1);
	}
}