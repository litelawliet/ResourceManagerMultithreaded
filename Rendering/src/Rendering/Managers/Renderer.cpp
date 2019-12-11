#include <stdafx.h>
#include <Rendering/Managers/Renderer.h>
#include <Rendering/Resources/Model.h>
#include "Rendering/Resources/ResourceManager.h"

void Rendering::Managers::Renderer::Clear() const noexcept
{
	m_driver->Clear();
}

void Rendering::Managers::Renderer::PolygonModeLine() noexcept
{
	m_fillDrawIsOn = false;
	m_driver->PolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Rendering::Managers::Renderer::PolygonModeFill() noexcept
{
	m_fillDrawIsOn = true;
	m_driver->PolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Rendering::Managers::Renderer::Draw(
	Resources::Model* p_model) const noexcept
{
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	if (p_model == nullptr)
	{
		return;
	}

	Resources::Mesh* meshPtr = p_model->GetMesh();
	if (meshPtr == nullptr)
	{
		p_model->SetMesh(Resources::ResourceManager::Instance().Get<Resources::Mesh>(p_model->GetModelPath()));
		meshPtr = p_model->GetMesh();
		if (meshPtr == nullptr)
			return;
	}

	if (!Resources::ResourceManager::Instance().IsMeshLoading(p_model->GetModelPath()) && !p_model->GetMesh()->IsMeshLoaded())
	{
		meshPtr->CreateBuffers();
	}
	
	p_model->Bind();

	if (meshPtr != nullptr && p_model->GetMesh()->GetIndicesCount() > 0 && meshPtr->IsVAOGenerated())
	{
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(meshPtr->GetIndicesCount()),
			GL_UNSIGNED_INT, nullptr);
	}
	else if (meshPtr != nullptr)
	{
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(meshPtr->GetVertexCount()));
	}

	p_model->Unbind();
}

bool Rendering::Managers::Renderer::IsDrawFilled() const
{
	return m_fillDrawIsOn;
}
