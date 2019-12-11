#include <stdafx.h>
#include <Rendering/Resources/Services/MeshManager.h>
#include <Rendering/Resources/Loaders/MeshLoader.h>
#include <Rendering/Utils/Colorizer.h>

Rendering::Resources::Services::MeshManager::~MeshManager()
{
	for (auto& mesh : m_resources)
	{
		delete mesh.second.first;
	}
}

void Rendering::Resources::Services::MeshManager::Add(const std::string& p_meshFilePath)
{
	const std::string objectName = GetObjectNameFromPath(p_meshFilePath);

	if (m_resources.find(objectName) == m_resources.end())
	{			
		m_resources.insert({ objectName, std::make_pair<Mesh*, bool>(new Mesh(objectName), true) });
		
		const Loaders::MeshLoader loader{ p_meshFilePath };

		m_resources.at(objectName).first->GetLoaderData(loader);
		m_resources.at(objectName).second = false;
	}
	else
	{
		Utils::Pencil::TextColor(Utils::EPencilColor::YELLOW);
		std::cerr <<
			"Warning: Mesh already exist in the resource manager, Add(...) operation ignored.\n";
		Utils::Pencil::TextColor(Utils::EPencilColor::WHITE);
	}
}

Rendering::Resources::Mesh* Rendering::Resources::Services::MeshManager::
Get(const std::string& p_meshName) const
{
	if (m_resources.find(p_meshName) != m_resources.end())
	{
		return m_resources.at(p_meshName).first;
	}

	return nullptr;
}

bool Rendering::Resources::Services::MeshManager::IsLoading(
	const std::string& p_resourceName)
{
	return m_resources.find(p_resourceName)->second.second;
}

bool Rendering::Resources::Services::MeshManager::LoadingsComplete()
{
	bool result = false;
	for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
	{
		result |= it->second.second;
	}

	return !result;
}
