#include <Core/Scene.h>
#include <Core/GameObject.h>
#include <Rendering/Managers/Renderer.h>
#include <Rendering/LowRenderer/Camera.h>
#include <Core/Components/ModelComponent.h>
#include <Core/Components/LightComponent.h>
#include <iostream>

std::vector<Core::Components::LightComponent*> Core::Scene::m_lightsInScene{};// = std::vector<Core::Components::LightComponent*>();

Core::Scene::Scene(const Scene & p_other)
{
	m_allGameObjects = p_other.m_allGameObjects;
}

void Core::Scene::AddGameObject(std::shared_ptr<GameObject> p_gameObject, const std::string& p_name)
{
	try
	{
		m_allGameObjects.try_emplace(p_name, p_gameObject);
		p_gameObject->SetName(p_name);
	}
	catch (...)
	{
		std::cerr << p_name + " is already added to Scene\n";
	}
}

void Core::Scene::RemoveGameObject(std::shared_ptr<GameObject>& p_gameObject)
{
	m_allGameObjects.erase(m_allGameObjects.find(p_gameObject->GetName()));
}

void Core::Scene::DrawAll(Rendering::Managers::Renderer& p_renderer)
{
	if (m_allGameObjects.empty())
		return;

	for (auto& gameObject : m_allGameObjects)
	{
		if (gameObject.second->GetComponent<Components::ModelComponent>() == nullptr)
			continue;

		p_renderer.Draw(gameObject.second->GetComponent<Components::ModelComponent>()->GetModel());
	}
}

void Core::Scene::Update(const double p_dt)
{
	m_lightsInScene.clear();
	for (auto& obj : m_allGameObjects)
	{
		const auto& lightObj = obj.second->GetComponent<Components::LightComponent>();
		if (lightObj != nullptr)
		{
			m_lightsInScene.push_back(&(*lightObj));
		}
	}

	UpdateAllGameObjects(p_dt);

}


void Core::Scene::UpdateAllGameObjects(const double p_dt)
{
	for (auto& gameObject : m_allGameObjects)
	{
		gameObject.second->Update(p_dt);
	}
}

void Core::Scene::UpdateAllComponents(const double p_dt)
{
	for (auto& gameObject : m_allGameObjects)
		gameObject.second->UpdateAllComponents(p_dt);

}

void Core::Scene::UpdateAllShaders()
{
	for (auto& gameObject : m_allGameObjects)
		gameObject.second->UpdateShaders();
}

void Core::Scene::RecompileAllShaders()
{
	for (auto& gameObject : m_allGameObjects)
		gameObject.second->RecompileShaders();
}

const std::vector<Core::Components::LightComponent*>& Core::Scene::GetLightsInCurrentScene()
{
	return m_lightsInScene;
}

std::shared_ptr<Core::GameObject> Core::Scene::FindGameObject(const std::string& p_name)
{
	if (m_allGameObjects.find(p_name) == m_allGameObjects.end())
	{
		std::cerr << "Could not find GameObject: " + p_name + " in Scene\n";
		return nullptr;
	}
	return m_allGameObjects.find(p_name)->second;
}


std::unordered_map<std::string, std::shared_ptr<Core::GameObject>>& Core::Scene::GetAllGameObjects()
{
	return m_allGameObjects;
}


