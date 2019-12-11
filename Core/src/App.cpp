#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include <Rendering/Context/OpenGL/GLFWDevice.h>
#include <Rendering/Context/OpenGL/GLEWDriver.h>
#include <Rendering/Managers/Renderer.h>
#include <Rendering/LowRenderer/Camera.h>
#include <Core/GameObject.h>
#include <Rendering/Managers/InputManager.h>
#include <Core/Scene.h>
#include <Core/GameManager.h>
#include <Core/Components/ModelComponent.h>
#include <Core/Components/CameraComponent.h>
#include <Core/Components/LightComponent.h>
#include <Rendering/Resources/ResourceManager.h>


int main()
{
	//_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	auto device = std::make_unique<Rendering::Context::OpenGL::GLFWDevice>(1024, 768);
	std::unique_ptr<Rendering::Managers::Renderer> renderer = std::make_unique<Rendering::Managers::Renderer>();
	renderer->Initialize<Rendering::Context::OpenGL::GLEWDriver>();

	Rendering::Managers::InputManager::InitializeFirstInstance(device->GetWindow());

	Core::GameManager gameManager(renderer.get());
	
	// #### Resource Manager
	Rendering::Resources::ResourceManager::Instance().Add<Rendering::Resources::Mesh>("../rsc/models/BarrocMiniTable.obj");
	Rendering::Resources::ResourceManager::Instance().Add<Rendering::Resources::Mesh>("../rsc/models/statue.obj");
	Rendering::Resources::ResourceManager::Instance().Add<Rendering::Resources::Mesh>("../rsc/models/dague.obj");
	Rendering::Resources::ResourceManager::Instance().Add<Rendering::Resources::Mesh>("../rsc/models/Lambo.obj");
	Rendering::Resources::ResourceManager::Instance().Add<Rendering::Resources::Mesh>("../rsc/models/cube.obj");

	Rendering::LowRenderer::Camera::GetInstance()->SetSensitivitySpeed(20.0f);

	std::shared_ptr<Core::GameObject> cube = std::make_shared<Core::GameObject>();
	std::shared_ptr<Core::GameObject> statue = std::make_shared<Core::GameObject>();
	std::shared_ptr<Core::GameObject> Lambo = std::make_shared<Core::GameObject>();
	std::shared_ptr<Core::GameObject> dague = std::make_shared<Core::GameObject>();
	std::shared_ptr<Core::GameObject> greatsword = std::make_shared<Core::GameObject>();
	std::shared_ptr<Core::GameObject> buildingsLight = std::make_shared<Core::GameObject>();

	Core::Scene scene1{};

	//Rendering::Resources::ResourceManager::Instance().WaitLoading();
	
   	Lambo->AddComponent<Core::Components::ModelComponent>("Lambo.obj");
	cube->AddComponent<Core::Components::ModelComponent>("cube.obj");
	statue->AddComponent<Core::Components::ModelComponent>("statue.obj");
	dague->AddComponent<Core::Components::ModelComponent>("dague.obj");
	greatsword->AddComponent<Core::Components::ModelComponent>("BarrocMiniTable.obj");

	//cube->AddTexture("../rsc/textures/Tiles_026_basecolor.png");
	//cube->AddNormalMap("../rsc/textures/Tiles_026_normal.png");
	//statue->AddTexture("../rsc/textures/Terracota_Tiles_003_basecolor.png");
	//statue->AddNormalMap("../rsc/textures/Terracota_Tiles_003_normal.png");
	//dague->AddTexture("../rsc/textures/bullet.png");
	//dague->AddNormalMap("../rsc/textures/metal_bullet.png");
	//greatsword->AddTexture("../rsc/textures/Tiles_026_basecolor.png");
	//greatsword->AddNormalMap("../rsc/textures/Tiles_026_normal.png");
	
	scene1.AddGameObject(cube, "cube");
	scene1.AddGameObject(statue, "statue");
	scene1.AddGameObject(Lambo, "car");
	scene1.AddGameObject(dague, "dague");
	scene1.AddGameObject(greatsword, "greatsword");
	scene1.AddGameObject(buildingsLight, "buildingsLight");

	gameManager.AddScene(scene1);
	gameManager.SetActiveScene(0);

	glm::vec3 cubePos = glm::vec3(0.5f, -0.00f, 0.0f);
	glm::vec3 cubeRot = glm::vec3(0, 0, 0);
	glm::vec3 cubeScale = glm::vec3(0.2f, 0.2f, 0.2f);

	glm::vec3 carPosition = glm::vec3(-0.5f, 0, 0.0f);
	glm::vec3 carRotation = glm::vec3(0, 0, 0);
	glm::vec3 carScale = glm::vec3(0.001f, 0.001f, 0.001f);

	glm::vec3 statuePos = glm::vec3(0.9f, -0.0f, 0.0f);
	glm::vec3 statueRot = glm::vec3(0, 0, 0);
	glm::vec3 statueScale = glm::vec3(0.001f, 0.001f, 0.001f);

	glm::vec3 daguePos = glm::vec3(-1.0f, -0.0f, 0.0f);
	glm::vec3 dagueRot = glm::vec3(90, 0, 0);
	glm::vec3 dagueScale = glm::vec3(0.1f, 0.1f, 0.1f);

	glm::vec3 greatswordPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 greatswordRot = glm::vec3(0, 0, 0);
	glm::vec3 greatswordScale = glm::vec3(0.005f, 0.005f, 0.005f);

	glm::vec3 buildingsLightPos = glm::vec3(-5.0f, -0.0f, 0.0f);
	glm::vec3 buildingsLightRotation = glm::vec3(0, 0, 0);
	glm::vec3 buildingsLightScale = glm::vec3(1.0f, 1.0f, 1.0f);

	statue->AddComponent<Core::Components::LightComponent>(glm::vec3{ 1.0f, 1.0f, 1.0f });
	buildingsLight->AddComponent<Core::Components::LightComponent>(glm::vec3{ 1.0f, 1.f, 1.0f });

	// Buildings light
	const auto& lightBuilding = buildingsLight->GetComponent<Core::Components::LightComponent>();
	lightBuilding->SetType(Core::Components::LIGHT_TYPE::POINT_LIGHT);
	lightBuilding->SetDirection(normalize(glm::vec3(-0.5f, -0.5f, -0.5f)));
	lightBuilding->SetConstant(1.0f);
	lightBuilding->SetLinear(0.5f);
	lightBuilding->SetQuadratic(0.2f);
	lightBuilding->SetIntensity(1.0f);

	// Settings the player light
	const auto& light = statue->GetComponent<Core::Components::LightComponent>();
	light->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	light->SetDirection(normalize(glm::vec3(-0.5f, -0.5f, -0.5f)));
	light->SetType(Core::Components::LIGHT_TYPE::DIRECTIONAL_LIGHT);
	light->SetConstant(1.0f);
	light->SetLinear(0.5f);
	light->SetQuadratic(0.2f);
	light->SetCutoff(0.0f);
	light->SetOuterCutoff(1.0f);

	Core::GameManager::GetActiveScene()->FindGameObject("car")->SetTransform(carPosition, carRotation, carScale);
	Core::GameManager::GetActiveScene()->FindGameObject("statue")->SetTransform(statuePos, statueRot, statueScale);
	Core::GameManager::GetActiveScene()->FindGameObject("cube")->SetTransform(cubePos, cubeRot, cubeScale);
	Core::GameManager::GetActiveScene()->FindGameObject("dague")->SetTransform(daguePos, dagueRot, dagueScale);
	Core::GameManager::GetActiveScene()->FindGameObject("greatsword")->SetTransform(greatswordPos, greatswordRot, greatswordScale);
	Core::GameManager::GetActiveScene()->FindGameObject("buildingsLight")->SetTransform(buildingsLightPos, buildingsLightRotation, buildingsLightScale);

	double currentTime = 0.0;
	double dt = 0.0;
	double lastTime = glfwGetTime();

	while (!device->ShouldClose())
	{
		currentTime = glfwGetTime();
		dt = currentTime - lastTime;
		lastTime = currentTime;
		device->RefreshEvent();
		renderer->Clear();

		// ##### Game logic #####
		gameManager.HandleEvents(dt);

		// ##### Update #####
		gameManager.Update(dt);

		// ##### Drawing #####
		gameManager.DrawActiveScene(*renderer);
		//fontLoader.RenderText(fontShader, "Test Text 1234", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		device->Render();
	}

	//_CrtDumpMemoryLeaks();
	return EXIT_SUCCESS;
}
