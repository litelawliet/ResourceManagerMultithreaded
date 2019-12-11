#pragma once
#include <Rendering/LowRenderer/Camera.h>
#include <Core/GameObject.h>
#include <glm/glm.hpp>

namespace Core::Components
{
	class CameraComponent : public IComponent
	{
	public:
		CameraComponent(GameObject& p_gameObject, glm::vec3& p_distanceFromPlayer);
		~CameraComponent() = default;

		void Update(const double p_dt) override;

	private:
		GameObject& m_gameObject;
		glm::vec3 m_distanceFromPlayer;
	};
}
