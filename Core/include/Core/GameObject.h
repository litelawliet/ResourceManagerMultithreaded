#pragma once
#include <Core/Components/IComponent.h>
#include <Rendering/Resources/Transform.h>
#include <vector>
#include <memory>
#include <iostream>
#include <string>

namespace Core
{
	namespace Components {
		class CameraComponent;
		class LightComponent;
		class PlayerComponent;
	}

	class GameObject
	{
	public:
		GameObject() = default;
		~GameObject() = default;
		GameObject(const GameObject& p_other);

		// p_vec : world space to move object to
		void MoveTo(glm::vec3& p_vec);
		// move object in p_vec direction
		void TranslateBy(glm::vec3& p_vec);

		Rendering::Resources::Transform& GetTransform() { return m_transform; }

		void SetTransform(glm::vec3 & p_pos, glm::vec3 & p_rot, glm::vec3 & p_scale);
		void SetTransform(Rendering::Resources::Transform & p_transform);

		void AddTexture(const std::string & p_texturePath) const;
		void AddNormalMap(const std::string & p_texturePath) const;

		void SetName(const std::string& p_name);
		std::string GetName() const;

		void Update(const double p_dt);
		void UpdateAllComponents(const double p_dt);
		void UpdateShaders() const;
		void RecompileShaders() const;

		//make into variadic template
		template<typename T>
		void AddComponent(T p_component)
		{
			try
			{
				if (typeid(T) == typeid(Core::Components::CameraComponent) && GetComponent<Core::Components::CameraComponent>() != nullptr)
				{
					std::cout << "GameObject already has one CameraComponent\n";
					return;
				}
				if (typeid(T) == typeid(Core::Components::LightComponent) && GetComponent<Core::Components::LightComponent>() != nullptr)
				{
					std::cerr << "GameObject already has one LightComponent\n";
					return;
				}
				m_components.emplace_back(std::make_shared<T>(p_component));
			}
			catch (...)
			{
				std::cout << "AddComponent didn't work\n";
			}
		}

		template<typename T, typename ... args>
		void AddComponent(args ... p_args)
		{
			try
			{
				if (typeid(T) == typeid(Core::Components::CameraComponent) && GetComponent<Core::Components::CameraComponent>() != nullptr)
				{
					std::cerr << "GameObject already has one CameraComponent\n";
					return;
				}
				if (typeid(T) == typeid(Core::Components::LightComponent) && GetComponent<Core::Components::LightComponent>() != nullptr)
				{
					std::cerr << "GameObject already has one LightComponent\n";
					return;
				}
				m_components.emplace_back(std::make_shared<T>(*this, p_args ...));
			}
			catch (...)
			{
				std::cout << "AddComponent didn't work\n";
			}
		}

		template<typename T>
		[[nodiscard]] std::shared_ptr<T> GetComponent() const noexcept
		{
			static_assert(std::is_base_of_v<Components::IComponent, T>);

			for (auto& component : m_components)
			{
				if (typeid(*component) == typeid(T))
				{
					return std::static_pointer_cast<T>(component);
				}
			}

			return {};
		}

		std::vector<std::shared_ptr<Core::Components::IComponent>>& GetComponents();

	private:
		std::vector<std::shared_ptr<Core::Components::IComponent>> m_components;
		Rendering::Resources::Transform m_transform{};
		std::string m_name;
	};
}
