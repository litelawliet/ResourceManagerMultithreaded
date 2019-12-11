#pragma once
#include <Core/GameObject.h>
#include <Rendering/Resources/Model.h>

namespace Core::Components
{
	class ModelComponent : public IComponent
	{
	public:
		explicit ModelComponent(GameObject& p_gameObject);
		ModelComponent(GameObject& p_gameObject, const std::string& p_path);
		ModelComponent(GameObject& p_gameObject, Rendering::Resources::Mesh* p_mesh);
		~ModelComponent();
		ModelComponent(const ModelComponent& p_other);

		void Update(const double p_dt) override;
		const Rendering::Resources::Mesh* GetMesh() const;
		Rendering::Resources::Model* GetModel() const;

	private:
		std::string m_type = "ModelComponent";
		Rendering::Resources::Model* m_model;
		GameObject& m_gameObject;
	};
}
