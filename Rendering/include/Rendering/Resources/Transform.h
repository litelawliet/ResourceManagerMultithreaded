#pragma once
#include <Rendering/Export.h>
#include <glm/glm.hpp>

namespace Rendering::Resources
{
	class API_RENDERING Transform
	{
	public:
		Transform(const glm::vec3& p_pos = glm::vec3{0.0f, 0.0f, 0.0f},
		          const glm::vec3& p_rot = glm::vec3{0.0f, 0.0f, 0.0f},
		          const glm::vec3& p_scale = glm::vec3{1.0f, 1.0f, 1.0f});
		explicit Transform(const Transform& p_other);

		glm::vec3& GetPos();
		glm::vec3& GetRot();
		glm::vec3& GetScale();

		void SetPos(const glm::vec3& p_pos);
		void SetRot(const glm::vec3& p_rot);
		void SetScale(const glm::vec3& p_scale);
		void SetTransform(const glm::vec3& p_pos, const glm::vec3& p_rot, const glm::vec3& p_scale);

		inline const glm::mat4& GetModelMatrix() const noexcept;
		void UpdateModelMatrix();

	private:
		glm::vec3 m_pos;
		glm::vec3 m_rot;
		glm::vec3 m_scale;
		glm::mat4 m_modelMatrix;
	};
}
