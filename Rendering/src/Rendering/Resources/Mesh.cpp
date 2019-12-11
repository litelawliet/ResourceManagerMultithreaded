#include <stdafx.h>
#include <Rendering/Resources/Mesh.h>

Rendering::Resources::Mesh::Mesh(const std::string& p_filePath)
	: m_name{ p_filePath.substr(p_filePath.find_last_of('/') + 1,
					std::string::npos) }
{
}

Rendering::Resources::Mesh::Mesh(Loaders::MeshLoader& p_meshLoader) noexcept
	: m_vertexCount{ p_meshLoader.GetVerticesCount() },
	m_indicesCount{ p_meshLoader.GetIndicesCount() }
{
	m_vertices = p_meshLoader.GetVertices();
	m_indices = p_meshLoader.GetIndices();
}

Rendering::Resources::Mesh::Mesh(const Mesh& p_mesh)
{
	m_vertexArray = p_mesh.m_vertexArray;

	m_texture = p_mesh.m_texture;
	m_normalMap = p_mesh.m_normalMap;

	m_indexBuffer = p_mesh.m_indexBuffer;
	m_vboTextCoords = p_mesh.m_vboTextCoords;
	m_vboPosition = p_mesh.m_vboPosition;
	m_vboTangent = p_mesh.m_vboTangent;
	m_vboBitangent = p_mesh.m_vboBitangent;
	m_vboNormal = p_mesh.m_vboNormal;

	m_name = p_mesh.m_name;
}

Rendering::Resources::Mesh::Mesh(Mesh&& p_mesh) noexcept
{
	m_vertexArray = p_mesh.m_vertexArray;

	m_texture = p_mesh.m_texture;
	m_normalMap = p_mesh.m_normalMap;

	m_indexBuffer = p_mesh.m_indexBuffer;
	m_vboTextCoords = p_mesh.m_vboTextCoords;
	m_vboPosition = p_mesh.m_vboPosition;
	m_vboTangent = p_mesh.m_vboTangent;
	m_vboBitangent = p_mesh.m_vboBitangent;
	m_vboNormal = p_mesh.m_vboNormal;

	m_name = p_mesh.m_name;
}

Rendering::Resources::Mesh::Mesh(const std::vector<Geometry::Vertex>& p_vertices,
	const std::vector<unsigned int>& p_indices)
	: m_vertexCount{ p_vertices.size() }, m_indicesCount{ p_indices.size() }
{
	m_vertices = p_vertices;
	m_indices = p_indices;
}

Rendering::Resources::Mesh::~Mesh() noexcept
{
	delete m_vboBitangent;
	delete m_vboTangent;
	delete m_vboNormal;
	delete m_vboTextCoords;
	delete m_vboPosition;

	delete m_indexBuffer;

	delete m_normalMap;
	delete m_texture;

	delete m_vertexArray;
}

void Rendering::Resources::Mesh::AddTexture(const std::string& p_texturePath) const
{
	if (m_texture != nullptr)
		m_texture->Load(p_texturePath, 1);
}

void Rendering::Resources::Mesh::AddNormalMap(const std::string& p_texturePath) const
{
	if (m_normalMap != nullptr)
		m_normalMap->Load(p_texturePath, 2);
}

void Rendering::Resources::Mesh::Bind() const noexcept
{
	if (m_vertexArray != nullptr)
		m_vertexArray->Bind();
	if (m_indexBuffer != nullptr)
		m_indexBuffer->Bind();
}

void Rendering::Resources::Mesh::BindTextures() const noexcept
{
	if (m_texture != nullptr)
		m_texture->Bind(0);
	if (m_normalMap != nullptr)
		m_normalMap->Bind(1);
}

void Rendering::Resources::Mesh::Unbind() const noexcept
{
	if (m_indexBuffer != nullptr)
		m_indexBuffer->Unbind();
	if (m_vertexArray != nullptr)
		m_vertexArray->Unbind();
}

void Rendering::Resources::Mesh::UnbindTextures() const noexcept
{
	if (m_normalMap != nullptr)
		m_normalMap->Unbind();
	if (m_texture != nullptr)
		m_texture->Unbind();
}

bool Rendering::Resources::Mesh::IsVAOGenerated() const
{
	return m_vertexArray != nullptr ? true : false;
}

size_t Rendering::Resources::Mesh::GetVertexCount() const noexcept
{
	return m_vertexCount;
}

size_t Rendering::Resources::Mesh::GetIndicesCount() const noexcept
{
	return m_indicesCount;
}

bool Rendering::Resources::Mesh::IsMeshLoaded() const noexcept
{
	return m_meshLoaded;
}

std::string Rendering::Resources::Mesh::GetName() const noexcept
{
	return m_name;
}

void Rendering::Resources::Mesh::CreateBuffers(const std::vector<Geometry::Vertex>& p_vertices,
	const std::vector<GLuint>& p_indices) noexcept
{
	m_vertexArray = new Buffers::VertexArray();
	m_texture = new Texture();
	m_normalMap = new Texture();
	m_vertexArray->Bind();

	std::vector<GLfloat> rawPositions{};
	std::vector<GLfloat> rawTextCoords{};
	std::vector<GLfloat> rawNormals{};
	std::vector<GLfloat> rawTangents{};
	std::vector<GLfloat> rawBitangents{};

	for (const auto& vertex : p_vertices)
	{
		// Position
		rawPositions.push_back(vertex.m_position.x);
		rawPositions.push_back(vertex.m_position.y);
		rawPositions.push_back(vertex.m_position.z);

		// TextCoord
		rawTextCoords.push_back(vertex.m_textCoords.x);
		rawTextCoords.push_back(vertex.m_textCoords.y);

		// Normal
		rawNormals.push_back(vertex.m_normal.x);
		rawNormals.push_back(vertex.m_normal.y);
		rawNormals.push_back(vertex.m_normal.z);

		rawTangents.push_back(vertex.m_tangent.x);
		rawTangents.push_back(vertex.m_tangent.y);
		rawTangents.push_back(vertex.m_tangent.z);

		rawBitangents.push_back(vertex.m_tangent.x);
		rawBitangents.push_back(vertex.m_tangent.y);
		rawBitangents.push_back(vertex.m_tangent.z);
	}

	// Buffer gen
	m_vboPosition = new Buffers::VertexBuffer(rawPositions);
	m_vboTextCoords = new Buffers::VertexBuffer(rawTextCoords);
	m_vboNormal = new Buffers::VertexBuffer(rawNormals);
	m_vboTangent = new Buffers::VertexBuffer(rawTangents);
	m_vboBitangent = new Buffers::VertexBuffer(rawBitangents);

	m_indexBuffer = new Buffers::IndexBuffer(p_indices);

	m_vertexArray->AddBuffer(*m_vboPosition, 3, Buffers::GLType::FLOAT);
	m_vertexArray->AddBuffer(*m_vboTextCoords, 2, Buffers::GLType::FLOAT);
	m_vertexArray->AddBuffer(*m_vboNormal, 3, Buffers::GLType::FLOAT);
	m_vertexArray->AddBuffer(*m_vboTangent, 3, Buffers::GLType::FLOAT);
	m_vertexArray->AddBuffer(*m_vboBitangent, 3, Buffers::GLType::FLOAT);

	m_vertexArray->Unbind();
}

void Rendering::Resources::Mesh::CreateBuffers() noexcept
{
	if (this != nullptr)
	{
		m_vertexArray = new Buffers::VertexArray();
		m_texture = new Texture();
		m_normalMap = new Texture();
		m_vertexArray->Bind();

		std::vector<GLfloat> rawPositions{};
		std::vector<GLfloat> rawTextCoords{};
		std::vector<GLfloat> rawNormals{};
		std::vector<GLfloat> rawTangents{};
		std::vector<GLfloat> rawBitangents{};

		for (const auto& vertex : m_vertices)
		{
			// Position
			rawPositions.push_back(vertex.m_position.x);
			rawPositions.push_back(vertex.m_position.y);
			rawPositions.push_back(vertex.m_position.z);

			// TextCoord
			rawTextCoords.push_back(vertex.m_textCoords.x);
			rawTextCoords.push_back(vertex.m_textCoords.y);

			// Normal
			rawNormals.push_back(vertex.m_normal.x);
			rawNormals.push_back(vertex.m_normal.y);
			rawNormals.push_back(vertex.m_normal.z);

			rawTangents.push_back(vertex.m_tangent.x);
			rawTangents.push_back(vertex.m_tangent.y);
			rawTangents.push_back(vertex.m_tangent.z);

			rawBitangents.push_back(vertex.m_tangent.x);
			rawBitangents.push_back(vertex.m_tangent.y);
			rawBitangents.push_back(vertex.m_tangent.z);
		}

		// Buffer gen
		m_vboPosition = new Buffers::VertexBuffer(rawPositions);
		m_vboTextCoords = new Buffers::VertexBuffer(rawTextCoords);
		m_vboNormal = new Buffers::VertexBuffer(rawNormals);
		m_vboTangent = new Buffers::VertexBuffer(rawTangents);
		m_vboBitangent = new Buffers::VertexBuffer(rawBitangents);

		m_indexBuffer = new Buffers::IndexBuffer(m_indices);

		m_vertexArray->AddBuffer(*m_vboPosition, 3, Buffers::GLType::FLOAT);
		m_vertexArray->AddBuffer(*m_vboTextCoords, 2, Buffers::GLType::FLOAT);
		m_vertexArray->AddBuffer(*m_vboNormal, 3, Buffers::GLType::FLOAT);
		m_vertexArray->AddBuffer(*m_vboTangent, 3, Buffers::GLType::FLOAT);
		m_vertexArray->AddBuffer(*m_vboBitangent, 3, Buffers::GLType::FLOAT);

		m_vertexArray->Unbind();
		m_meshLoaded = true;
	}
}

void Rendering::Resources::Mesh::SetName(const std::string& p_name)
{
	m_name = p_name;
}

Rendering::Resources::Mesh& Rendering::Resources::Mesh::operator=(const Mesh& p_mesh)
{
	m_vertexArray = p_mesh.m_vertexArray;

	m_texture = p_mesh.m_texture;
	m_normalMap = p_mesh.m_normalMap;

	m_indexBuffer = p_mesh.m_indexBuffer;
	m_vboTextCoords = p_mesh.m_vboTextCoords;
	m_vboPosition = p_mesh.m_vboPosition;
	m_vboTangent = p_mesh.m_vboTangent;
	m_vboBitangent = p_mesh.m_vboBitangent;
	m_vboNormal = p_mesh.m_vboNormal;

	return *this;
}

void Rendering::Resources::Mesh::GetLoaderData(const Loaders::MeshLoader& p_loader)
{
	m_vertexCount = p_loader.GetVerticesCount();
	m_indicesCount = p_loader.GetIndicesCount();
	m_vertices = p_loader.GetVertices();
	m_indices = p_loader.GetIndices();
}