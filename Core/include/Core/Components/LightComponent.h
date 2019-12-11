#pragma once

#include <Core/Components/IComponent.h>
#include <Core/GameObject.h>
#include <Rendering/LowRenderer/Light.h>
#include <glm/glm.hpp>

namespace Core::Components
{
	enum class LIGHT_TYPE
	{
		DIRECTIONAL_LIGHT = 1,
		POINT_LIGHT = 2,
		SPOT_LIGHT = 3,
	};

	class LightComponent : public IComponent
	{
	public:
		explicit LightComponent(GameObject& p_gameObject, const glm::vec3 & p_color);
		~LightComponent() = default;
		LightComponent(const LightComponent& p_other);

		void Update(const double p_dt) override;
		void AddLight(const LIGHT_TYPE & p_type, const glm::vec3 & p_color = glm::vec3{ 1.0f }, const glm::vec3 & p_position = glm::vec3{ 0.0f }, const float p_intensity = 1.0f);

		// Preset of lights to parametrize
		void PresetDirectionalLight(const glm::vec3 & p_color = glm::vec3{ 1.0 }, const glm::vec3 & p_position = glm::vec3{ 0.0f }, const glm::vec3& p_direction = glm::vec3{ 0.0f }, const float p_intensity = 1.0f);
		void PresetPointLight(const float p_constant, const float p_linear, const float p_quadratic, const float p_intensity, const glm::vec3 & p_color = glm::vec3{ 1.0 }, const glm::vec3 & p_position = glm::vec3{ 0.0f }, const glm::vec3& p_direction = glm::vec3{ 0.0f });
		// NOT IMPLEMENTED:
		void PresetSpotLight();

		void SetConstant(const float p_constant);
		void SetLinear(const float p_linear);
		void SetQuadratic(const float p_quadratic);
		void SetCutoff(const float p_cutoff);
		void SetOuterCutoff(const float p_outerCutoff);
		void SetPosition(const glm::vec3 & p_position);
		void SetColor(const glm::vec3 & p_color);
		void SetDirection(const glm::vec3 & p_direction);
		void SetType(const LIGHT_TYPE & p_type);
		void SetIntensity(const float p_intensity);

		const Rendering::LowRenderer::LightSource& GetLight() const;

	private:
		GameObject& m_gameObject;
		Rendering::LowRenderer::LightSource m_light;
	};
}
