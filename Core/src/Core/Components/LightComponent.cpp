#include <Core/Components/LightComponent.h>

Core::Components::LightComponent::LightComponent(GameObject& p_gameObject, const glm::vec3 & p_color)
	: m_gameObject{ p_gameObject }, m_light{}
{
	m_light.Color = p_color;
	m_light.Type = static_cast<int>(LIGHT_TYPE::DIRECTIONAL_LIGHT);
	m_light.Intensity = 1.0f;
}

Core::Components::LightComponent::LightComponent(const LightComponent& p_other)
	: m_gameObject{ p_other.m_gameObject }, m_light{ p_other.m_light }
{
}

void Core::Components::LightComponent::Update(const double p_dt)
{
}

void Core::Components::LightComponent::AddLight(const LIGHT_TYPE & p_type, const glm::vec3& p_color, const glm::vec3& p_position, const float p_intensity)
{
	m_light.Type = static_cast<int>(p_type);
	m_light.Color = p_color;
	m_light.Position = p_position;
	m_light.Intensity = p_intensity;
}

void Core::Components::LightComponent::PresetDirectionalLight(const glm::vec3& p_color, const glm::vec3& p_position,
	const glm::vec3& p_direction, const float p_intensity)
{
	m_light.Type = static_cast<int>(LIGHT_TYPE::DIRECTIONAL_LIGHT);
	m_light.Color = p_color;
	m_light.Position = p_position;
	m_light.Direction = p_direction;
	m_light.Intensity = p_intensity;
}

void Core::Components::LightComponent::PresetPointLight(const float p_constant, const float p_linear,
	const float p_quadratic, const float p_intensity, const glm::vec3& p_color, const glm::vec3& p_position, const glm::vec3& p_direction)
{
	m_light.Type = static_cast<int>(LIGHT_TYPE::POINT_LIGHT);
	m_light.Constant = p_constant;
	m_light.Linear = p_linear;
	m_light.Quadratic = p_quadratic;
	m_light.Color = p_color;
	m_light.Position = p_position;
	m_light.Direction = p_direction;
	m_light.Intensity = p_intensity;
}

void Core::Components::LightComponent::PresetSpotLight()
{
	m_light.Type = static_cast<int>(LIGHT_TYPE::SPOT_LIGHT);
}

void Core::Components::LightComponent::SetConstant(const float p_constant)
{
	m_light.Constant = p_constant;
}

void Core::Components::LightComponent::SetLinear(const float p_linear)
{
	m_light.Linear = p_linear;
}

void Core::Components::LightComponent::SetQuadratic(const float p_quadratic)
{
	m_light.Quadratic = p_quadratic;
}

void Core::Components::LightComponent::SetCutoff(const float p_cutoff)
{
	m_light.Cutoff = p_cutoff;
}

void Core::Components::LightComponent::SetOuterCutoff(const float p_outerCutoff)
{
	m_light.OuterCutoff = p_outerCutoff;
}

void Core::Components::LightComponent::SetPosition(const glm::vec3& p_position)
{
	m_light.Position = p_position;
}

void Core::Components::LightComponent::SetColor(const glm::vec3& p_color)
{
	m_light.Color = p_color;
}

void Core::Components::LightComponent::SetDirection(const glm::vec3& p_direction)
{
	m_light.Direction = p_direction;
}

void Core::Components::LightComponent::SetType(const LIGHT_TYPE & p_type)
{
	m_light.Type = static_cast<int>(p_type);
}

void Core::Components::LightComponent::SetIntensity(const float p_intensity)
{
	m_light.Intensity = p_intensity;
}

const Rendering::LowRenderer::LightSource& Core::Components::LightComponent::GetLight() const
{
	return m_light;
}
