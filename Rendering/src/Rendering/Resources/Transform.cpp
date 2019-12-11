#include <stdafx.h>
#include <Rendering/Resources/Transform.h>
#include <glm/gtc/matrix_transform.hpp>

Rendering::Resources::Transform::Transform(const glm::vec3& p_pos, const glm::vec3& p_rot, const glm::vec3& p_scale)
	: m_pos(p_pos),
	  m_rot(p_rot),
	  m_scale(p_scale),
	  m_modelMatrix{1.0f}
{
	UpdateModelMatrix();
}

Rendering::Resources::Transform::Transform(const Transform& p_other)
	: m_pos{p_other.m_pos}, m_rot{p_other.m_rot}, m_scale{p_other.m_scale}, m_modelMatrix{p_other.m_modelMatrix}
{
	UpdateModelMatrix();
}

glm::vec3& Rendering::Resources::Transform::GetPos()
{
	return m_pos;
}

glm::vec3& Rendering::Resources::Transform::GetRot()
{
	return m_rot;
}

glm::vec3& Rendering::Resources::Transform::GetScale()
{
	return m_scale;
}

void Rendering::Resources::Transform::SetPos(const glm::vec3& p_pos)
{
	m_pos = p_pos;
	UpdateModelMatrix();
}

void Rendering::Resources::Transform::SetRot(const glm::vec3& p_rot)
{
	m_rot = p_rot;
	UpdateModelMatrix();
}

void Rendering::Resources::Transform::SetScale(const glm::vec3& p_scale)
{
	m_scale = p_scale;
	UpdateModelMatrix();
}

void Rendering::Resources::Transform::SetTransform(const glm::vec3& p_pos, const glm::vec3& p_rot,
                                                   const glm::vec3& p_scale)
{
	m_pos = p_pos;
	m_rot = p_rot;
	m_scale = p_scale;
	UpdateModelMatrix();
}

inline const glm::mat4& Rendering::Resources::Transform::GetModelMatrix() const noexcept
{
	return m_modelMatrix;
}

void Rendering::Resources::Transform::UpdateModelMatrix()
{
	m_modelMatrix = glm::mat4(1.0f);
	glm::mat4 posMat{1.0f};
	glm::mat4 scaleMat(1.0f);
	glm::mat4 rotX{1.0f};
	glm::mat4 rotY{1.0f};
	glm::mat4 rotZ{1.0f};
	posMat = translate(posMat, m_pos);
	scaleMat = scale(scaleMat, m_scale);
	rotX = rotate(rotX, glm::radians(m_rot.x), glm::vec3{1.0f, 0.0f, 0.0f});
	rotY = rotate(rotY, glm::radians(m_rot.y), glm::vec3{0.0f, 1.0f, 0.0f});
	rotZ = rotate(rotZ, glm::radians(m_rot.z), glm::vec3{0.0f, 0.0f, 1.0f});
	const glm::mat4 rotMat = rotX * rotY * rotZ;
	m_modelMatrix = posMat * rotMat * scaleMat;
}
