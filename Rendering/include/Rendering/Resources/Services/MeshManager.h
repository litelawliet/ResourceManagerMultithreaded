#pragma once

#include <Rendering/Export.h>
#include <string>
#include <unordered_map>
#include <Rendering/Resources/Mesh.h>
#include <concurrent_unordered_map.h>


namespace Rendering::Resources::Services
{
	class API_RENDERING MeshManager
	{
	public:
		MeshManager() = default;
		~MeshManager();

		void Add(const std::string& p_meshFilePath);

		Mesh* Get(const std::string& p_meshName) const;
		bool IsLoading(const std::string& p_resourceName);
		bool LoadingsComplete();

	private:
		Concurrency::concurrent_unordered_map<std::string, std::pair<Mesh*, bool>> m_resources;

		[[nodiscard]] static std::string GetObjectNameFromPath(
			const std::string& p_objectPath)
		{
			return p_objectPath.substr(p_objectPath.find_last_of('/') + 1,
				std::string::npos);
		}
	};
}
