#include <stdafx.h>
#include <Core/Components/ModelComponent.h>
#include <Rendering/LowRenderer/Camera.h>

Core::Components::ModelComponent::ModelComponent(GameObject& p_gameObject) : m_gameObject{ p_gameObject }
{
	m_model = nullptr;
	m_type = "Model";
}

Core::Components::ModelComponent::ModelComponent(GameObject& p_gameObject, const std::string& p_path) : m_gameObject{ p_gameObject }
{
	m_model = new Rendering::Resources::Model(p_path);
}

Core::Components::ModelComponent::ModelComponent(GameObject& p_gameObject, Rendering::Resources::Mesh* p_mesh) : m_gameObject{ p_gameObject }
{
	m_model = new Rendering::Resources::Model(p_mesh);
}

Core::Components::ModelComponent::~ModelComponent()
{
	delete m_model;
}

Core::Components::ModelComponent::ModelComponent(const ModelComponent & p_other) : m_gameObject{ p_other.m_gameObject }
{
	m_type = p_other.m_type;
	m_model = p_other.m_model;
}

void Core::Components::ModelComponent::Update(const double p_dt)
{
	const glm::mat4 projection = Rendering::LowRenderer::Camera::GetInstance()->GetProjectionMatrix();
	const glm::mat4 view = Rendering::LowRenderer::Camera::GetInstance()->GetViewMatrix();

	const glm::mat4 modelMatrix = m_gameObject.GetTransform().GetModelMatrix();

	if (m_model->GetShader() != nullptr)
	{
		m_model->GetShader()->Bind();
		m_model->GetShader()->SetUniformMatrix4fv("u_ModelMatrix", modelMatrix);
		m_model->GetShader()->SetUniformMatrix4fv("u_ProjectionMatrix", projection);
		m_model->GetShader()->SetUniformMatrix4fv("u_ViewMatrix", view);
		m_model->GetShader()->Unbind();
	}
}

const Rendering::Resources::Mesh* Core::Components::ModelComponent::GetMesh() const
{
	return m_model->GetMesh();
}

Rendering::Resources::Model* Core::Components::ModelComponent::GetModel() const
{
	return m_model;
}
