#pragma once

#include <string_view>
#include <Rendering/Export.h>
#include <unordered_map>
#include <glm/glm.hpp>


namespace Rendering::Resources
{
	class API_RENDERING Shader
	{
	public:

		Shader() = delete;

		/**
		 * Use this constructor for a custom shader.
		 * @param p_vertexFilepath
		 * @param p_fragmentFilepath
		 */
		Shader(const std::string& p_vertexFilepath,
		       const std::string& p_fragmentFilepath);
		~Shader();

		void Bind() const;
		void Unbind() const;
		void SetUniform3f(const std::string& p_name, const float p_v0, const float p_v1, const float p_v2);

		// utility uniform functions
		void SetBool(const std::string& p_name, const bool p_value);
		void SetInt(const std::string& p_name, const int p_value);
		void SetFloat(const std::string& p_name, const float p_value);
		void SetUniform4f(const std::string& p_name, const float p_v0,
		                  const float p_v1, const float p_v2,
		                  const float p_v3);

		void SetUniformMatrix4fv(const std::string& p_name, const glm::mat4& p_matrix);

		void RecompileShader();

		inline GLuint GetId() const;
		inline const std::string& GetVertexShaderPath() const;
		inline const std::string& GetFragmentShaderPath() const;


	private:
		[[nodiscard]] static std::string ShaderProgramSource(
			const std::string_view& p_filepath);
		[[nodiscard]] static GLuint CompileShader(const GLuint& p_type,
		                                          const std::string& p_source);

		int GetUniformLocation(const std::string& p_name);


		GLuint m_id{0u};
		std::string m_sourceVertexShader;
		std::string m_sourceFragmentShader;
		std::unordered_map<std::string, GLint> m_uniformLocationCache;
	};
}
