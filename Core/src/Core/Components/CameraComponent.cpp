#include <Core/Components/CameraComponent.h>

Core::Components::CameraComponent::CameraComponent(GameObject& p_gameObject, glm::vec3& p_distanceFromPlayer) :
	m_gameObject { p_gameObject }, m_distanceFromPlayer { p_distanceFromPlayer }
{
	glm::vec3 newPos = m_gameObject.GetTransform().GetPos() + m_distanceFromPlayer;
	Rendering::LowRenderer::Camera::GetInstance()->SetPos(newPos);
}

void Core::Components::CameraComponent::Update(const double p_dt)
{
	Rendering::LowRenderer::Camera::GetInstance()->GetViewMatrix();
	glm::vec3 newPos = m_gameObject.GetTransform().GetPos() + m_distanceFromPlayer;
	Rendering::LowRenderer::Camera::GetInstance()->SetPos(newPos);
}

