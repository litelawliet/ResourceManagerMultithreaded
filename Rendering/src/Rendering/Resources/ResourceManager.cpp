#include <stdafx.h>
#include <Rendering/Resources/ResourceManager.h>

Rendering::Resources::ResourceManager& Rendering::Resources::ResourceManager::Instance()
{
	// Lifetime of a static inside a function explanation:
	// They are called Magic statics since C++11 because they are thread-safe initialized
	// They can be initialized at the first time anywhere in a function and will
	// be accessible and running until the program termination.
	//
	// This is a lazy-evaluated singleton, thread-safe and correctly-destroyed
	static ResourceManager m_instance;

	return m_instance;
}

void Rendering::Resources::ResourceManager::WaitLoading()
{
	do
	{
	} while (!m_meshManager.LoadingsComplete());
}

bool Rendering::Resources::ResourceManager::IsMeshLoading(const std::string& p_resourceName)
{
	return m_meshManager.IsLoading(p_resourceName);
}
