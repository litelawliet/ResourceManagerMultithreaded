#include <Core/GameManager.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Rendering/Managers/InputManager.h>
#include <Rendering/LowRenderer/Camera.h>

using namespace Core;

Scene* GameManager::m_activeScene = nullptr;

GameManager::GameManager(Rendering::Managers::Renderer * p_renderer)
	: m_rendererRef{ p_renderer }
{
}

GameManager::~GameManager()
= default;

void GameManager::AddGameObjectToScene(const std::string& p_name, std::shared_ptr<GameObject> p_gameObject, const unsigned int p_scene)
{
	m_allScenes[p_scene].GetAllGameObjects().try_emplace(p_name, p_gameObject);
}

void GameManager::AddScene(Scene & p_scene)
{
	m_allScenes.push_back(p_scene);
}

void GameManager::SetActiveScene(unsigned int p_activeScene)
{
	if (p_activeScene > m_allScenes.size() - 1)
	{
		std::cerr << "Cannot set " << p_activeScene << " as active scene, since total amount of scenes is " << m_allScenes.size() << '\n';
		return;
	}

	m_activeSceneIndex = p_activeScene;
	m_activeScene = &(m_allScenes[m_activeSceneIndex]);
}

std::vector<Scene>& GameManager::GetScenes() noexcept
{
	return m_allScenes;
}

Scene * GameManager::GetActiveScene() noexcept
{
	return m_activeScene;
}

unsigned int GameManager::GetActiveSceneNumber() const noexcept
{
	return m_activeSceneIndex;
}

void GameManager::HandleEvents(const double p_dt)
{
	Rendering::LowRenderer::Camera* cameraInstance = Rendering::LowRenderer::Camera::GetInstance().get();
	Rendering::Managers::InputManager* inputManagerInstance = Rendering::Managers::InputManager::GetInstance().get();

	cameraInstance->Move(*Rendering::Managers::InputManager::GetInstance(), p_dt);
	cameraInstance->LookAround(*Rendering::Managers::InputManager::GetInstance(), p_dt);

	// Comment the line below to disable the Player 3D objects to move with the Camera
	// Note: it will keep rotating around, to disable the rotation of the 3D model go to the Update method of PlayerComponent
	//m_allScenes[m_activeSceneIndex].FindGameObject("player")->GetComponent<Components::PlayerComponent>()->Move(p_dt);

	if (inputManagerInstance->GetKeyDown(Rendering::Utils::Event<int>::KeyCode::X))
	{
		m_allScenes[m_activeSceneIndex].RecompileAllShaders();
	}

	if (inputManagerInstance->GetKeyDown(Rendering::Utils::Event<int>::KeyCode::Z))
	{
		m_rendererRef->IsDrawFilled() ? m_rendererRef->PolygonModeLine() : m_rendererRef->PolygonModeFill();
	}

	inputManagerInstance->UpdateCursorPos();
}

void GameManager::Update(const double p_dt)
{
	const double currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	if (!m_allScenes.empty())
		m_allScenes[m_activeSceneIndex].Update(p_dt);
}

void GameManager::DrawActiveScene(Rendering::Managers::Renderer& p_renderer)
{
	if (!m_allScenes.empty())
		m_allScenes[m_activeSceneIndex].DrawAll(p_renderer);
}


std::unordered_map<std::string, std::shared_ptr<GameObject>>& GameManager::GetGameObjectsInScene(unsigned int p_scene)
{
	if (p_scene > m_allScenes.size() - 1)
	{
		std::cerr << "Cannot set " << p_scene << " as active scene, since total amount of scenes is " << m_allScenes.size() << '\n';
	}

	return m_allScenes[p_scene].GetAllGameObjects();
}