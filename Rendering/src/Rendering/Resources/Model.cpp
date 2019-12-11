#include <stdafx.h>
#include <Rendering/Resources/Model.h>
#include <Rendering/Resources/Mesh.h>
#include <Rendering/Resources/Loaders/ShaderLoader.h>
#include <Rendering/Resources/Loaders/MeshLoader.h>
#include "Rendering/Resources/ResourceManager.h"

Rendering::Resources::Model::Model(const std::string& p_path) noexcept
	: m_modelPath{ p_path }
{
	LoadModel(p_path);
	LoadShader();
}

Rendering::Resources::Model::Model(const std::string& p_modelPath,
	const std::string& p_vertexFilepath,
	const std::string& p_fragmentFilepath) noexcept
{
	LoadModel(p_modelPath);
	LoadShader(p_vertexFilepath, p_fragmentFilepath);
}

Rendering::Resources::Model::Model(Mesh* p_mesh)
{
	m_mesh = p_mesh;
 	LoadShader();
	if (m_mesh != nullptr)
		m_modelPath = p_mesh->GetName();
}

Rendering::Resources::Model::Model(const Model& p_other) noexcept
{
	m_modelPath = p_other.m_modelPath;
	m_mesh = p_other.m_mesh;
	m_shader = p_other.m_shader;
}

Rendering::Resources::Model::~Model()
{
	m_mesh = nullptr;
	if (!m_shader)
		delete m_shader;
}

void Rendering::Resources::Model::SetShader(const std::string& p_vertexFilepath,
	const std::string& p_fragmentFilepath) noexcept
{
	LoadShader(p_vertexFilepath, p_fragmentFilepath);
}

void Rendering::Resources::Model::AddTexture(const std::string& p_texturePath) const
{
	if (m_mesh != nullptr)
		m_mesh->AddTexture(p_texturePath);
}

void Rendering::Resources::Model::AddNormalMap(const std::string& p_texturePath) const
{
	if (m_mesh != nullptr)
		m_mesh->AddNormalMap(p_texturePath);
}

void Rendering::Resources::Model::LoadModel(const std::string& p_path) noexcept
{
	m_mesh = ResourceManager::Instance().Get<Mesh>(p_path);
	m_modelPath = p_path.substr(p_path.find_last_of('/') + 1,
		std::string::npos);
}

void Rendering::Resources::Model::LoadShader(const std::string& p_vertexFilepath,
	const std::string& p_fragmentFilepath) noexcept
{
	m_shader = Loaders::ShaderLoader::Load(p_vertexFilepath, p_fragmentFilepath);
}

void Rendering::Resources::Model::SetMesh(Mesh* p_mesh)
{
	m_mesh = p_mesh;
}

const std::string& Rendering::Resources::Model::GetModelPath() const
{
	return m_modelPath;
}

void Rendering::Resources::Model::Bind() const noexcept
{
	if (m_mesh != nullptr)
		m_mesh->Bind();
	
	if (m_shader != nullptr)
		m_shader->Bind();
	
	if (m_mesh != nullptr)
		m_mesh->BindTextures();
}

void Rendering::Resources::Model::Unbind() const noexcept
{
	if (m_mesh != nullptr)
		m_mesh->UnbindTextures();

	if (m_shader != nullptr)
		m_shader->Unbind();

	if (m_mesh != nullptr)
		m_mesh->Unbind();
}
