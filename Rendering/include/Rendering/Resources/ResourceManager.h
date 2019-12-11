#pragma once

#include <Rendering/Export.h>
#include <iostream>
#include <Rendering/Resources/Services/MeshManager.h>
#include <Rendering/Utils/Colorizer.h>
#include "Rendering/Utils/ThreadPool.h"

namespace Rendering::Resources
{
	class API_RENDERING ResourceManager
	{
	public:
		static ResourceManager& Instance();

		template<typename Resource>
		void Add(const std::string& p_resourceName)
		{
			if (typeid(Resource) == typeid(Mesh))
			{
				m_threadPool.AddTask(&ResourceManager::AddMT<Resource>, this, (p_resourceName));
			}
		}

		template<typename Resource>
		void AddMT(const std::string& p_resourceName)
		{
			if (typeid(Resource) == typeid(Mesh))
			{
				m_meshManager.Add(p_resourceName);
			}
			else
			{
				Utils::Pencil::TextColor(Utils::EPencilColor::YELLOW);
				std::cerr <<
					"Warning: This type of resource is not handle in the resource manager, Add(...) operation ignored.\n";
				Utils::Pencil::TextColor(Utils::EPencilColor::WHITE);
			}
		}

		template<typename Resource>
		Resource* Get(const std::string& p_resourceName)
		{
			if (typeid(Resource) == typeid(Mesh))
			{
				return m_meshManager.Get(p_resourceName);;
			}
			return nullptr;
		}

		void WaitLoading();
		bool IsMeshLoading(const std::string& p_resourceName);

		ResourceManager(ResourceManager const&) = delete;
		void operator=(ResourceManager const&) = delete;

	private:
		ResourceManager() = default;

		~ResourceManager()
		{
			//WaitLoading();
		}

		Services::MeshManager m_meshManager;
		Utils::ThreadPool m_threadPool;

		int m_threadsOnUse = 0;

		enum EService
		{
			MESH_SERVICE = 0,
			SHADER_SERVICE = 1,
		};
	};
}
