#pragma once
#include <Rendering/Export.h>
#include <Rendering/Resources/Mesh.h>
#include <Rendering/Resources/Shader.h>
#include <string>

namespace Rendering::Resources
{
	class API_RENDERING Model
	{
	public:
		Model() noexcept = default;
		explicit Model(const std::string& p_path) noexcept;
		Model(const std::string& p_modelPath,
			const std::string& p_vertexFilepath,
			const std::string& p_fragmentFilepath) noexcept;
		explicit Model(Mesh* p_mesh);
		Model(const Model& p_other) noexcept;
		~Model();


		void SetShader(const std::string& p_vertexFilepath =
			"../rsc/shaders/defaultShader.vert",
			const std::string& p_fragmentFilepath =
			"../rsc/shaders/defaultShader.frag") noexcept;
		void AddTexture(const std::string& p_texturePath) const;
		void AddNormalMap(const std::string& p_texturePath) const;

		inline Mesh* GetMesh() const noexcept { return m_mesh; }

		inline Shader* GetShader() const noexcept { return m_shader; }

		inline Texture* GetTexture() const noexcept { return m_mesh->GetTexture(); }
		void SetMesh(Mesh* p_mesh);

		inline Texture* GetNormalMap() const noexcept { return m_mesh->GetNormalMap(); }

		const std::string& GetModelPath() const;

		void Bind() const noexcept;
		void Unbind() const noexcept;

	private:
		std::string m_modelPath;

		Shader* m_shader{ nullptr };
		Mesh* m_mesh{ nullptr };

		unsigned int m_offset = 0;

		// We file these vector of vertices and indices to store the values of the mesh
		// then we send those 2 vectors into the mesh instantiation

		/*  Functions   */
		void LoadModel(const std::string& path) noexcept;

		/**
		 * @brief Method who can lod a shader. If no parameters are send, it loads both the default vertex and fragment shader.
		 * @param p_vertexFilepath : Vertex Shader to load.
		 * @param p_fragmentFilepath : Fragment Shader to load.
		**/
		void LoadShader(
			const std::string& p_vertexFilepath =
			"../rsc/shaders/defaultShader.vert",
			const std::string& p_fragmentFilepath =
			"../rsc/shaders/defaultShader.frag") noexcept;
	};
}
