#pragma once
#include <Rendering/Managers/Renderer.h>
#include <Core/GameObject.h>
#include <unordered_map>
#include <Core/Components/LightComponent.h>

namespace Core
{
	class Scene
	{
	public:
		Scene() = default;
		~Scene() = default;
		Scene(const Scene& p_other);

		void AddGameObject(std::shared_ptr<GameObject> p_gameObject, const std::string& p_name);
		void RemoveGameObject(std::shared_ptr<GameObject>& p_gameObject);
		std::shared_ptr<Core::GameObject> FindGameObject(const std::string& p_name);
		void DrawAll(Rendering::Managers::Renderer& p_renderer);

		void Update(const double p_dt);
		void UpdateAllGameObjects(const double p_dt);
		void UpdateAllComponents(const double p_dt);
		void UpdateAllShaders();
		void RecompileAllShaders();

		static const std::vector<Components::LightComponent*>& GetLightsInCurrentScene();
		std::unordered_map<std::string, std::shared_ptr<GameObject>>& GetAllGameObjects();

	private:
		std::unordered_map<std::string, std::shared_ptr<GameObject>> m_allGameObjects{};
		static std::vector<Components::LightComponent*> m_lightsInScene;
	};
}
