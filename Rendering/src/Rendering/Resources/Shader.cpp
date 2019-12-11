#include <stdafx.h>
#include <Rendering/Resources/Shader.h>

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <iostream>
#include <Rendering/Debug/ShaderDebug.h>

using namespace Rendering::Resources;

Shader::Shader(const std::string& p_vertexFilepath,
               const std::string& p_fragmentFilepath)
	: m_sourceVertexShader{std::move(p_vertexFilepath)}, m_sourceFragmentShader{std::move(p_fragmentFilepath)}
{
	const GLuint vertexShaderId =
		CompileShader(GL_VERTEX_SHADER, ShaderProgramSource(p_vertexFilepath));
	const GLuint fragmentShaderId =
		CompileShader(GL_FRAGMENT_SHADER, ShaderProgramSource(p_fragmentFilepath));
	m_id = glCreateProgram();

	glAttachShader(m_id, vertexShaderId);
	glAttachShader(m_id, fragmentShaderId);
	glLinkProgram(m_id);

	Debug::OpenGL::Shader::ShaderDebug::CheckLinkingShaderErrors(m_id);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
}

Shader::~Shader()
{
	glDeleteProgram(m_id);
}

void Shader::Bind() const
{
	glUseProgram(m_id);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SetUniform3f(const std::string& p_name, const float p_v0, const float p_v1, const float p_v2)
{
	glUniform3f(GetUniformLocation(p_name), p_v0, p_v1, p_v2);
}

void Shader::SetBool(const std::string& p_name, const bool p_value)
{
	glUniform1i(GetUniformLocation(p_name), static_cast<int>(p_value));
}

void Shader::SetInt(const std::string& p_name, const int p_value)
{
	glUniform1i(GetUniformLocation(p_name), p_value);
}

void Shader::SetFloat(const std::string& p_name, const float p_value)
{
	glUniform1f(GetUniformLocation(p_name), p_value);
}

void Shader::SetUniform4f(const std::string& p_name, const float p_v0,
                          const float p_v1, const float p_v2,
                          const float p_v3)
{
	glUniform4f(GetUniformLocation(p_name), p_v0, p_v1, p_v2, p_v3);
}

void Shader::SetUniformMatrix4fv(const std::string& p_name, const glm::mat4& p_matrix)
{
	glUniformMatrix4fv(GetUniformLocation(p_name), 1, GL_FALSE, glm::value_ptr(p_matrix));
}

void Shader::RecompileShader()
{
	glDeleteProgram(m_id);

	const GLuint vertexShaderId =
		CompileShader(GL_VERTEX_SHADER, ShaderProgramSource(m_sourceVertexShader));
	const GLuint fragmentShaderId =
		CompileShader(GL_FRAGMENT_SHADER, ShaderProgramSource(m_sourceFragmentShader));

	m_id = glCreateProgram();

	glAttachShader(m_id, vertexShaderId);
	glAttachShader(m_id, fragmentShaderId);
	glLinkProgram(m_id);

	Debug::OpenGL::Shader::ShaderDebug::CheckLinkingShaderErrors(m_id);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
}

GLint Shader::GetUniformLocation(const std::string& p_name)
{
	if (m_uniformLocationCache.find(p_name) != m_uniformLocationCache.end())
	{
		return m_uniformLocationCache[p_name];
	}
	const GLint location = glGetUniformLocation(m_id, p_name.c_str());

	if (location == -1)
	{
		std::cout << "Warning: uniform '" << p_name << "' doesn't exist!\n";
	}

	m_uniformLocationCache[p_name] = location;

	return location;
}


GLuint Shader::CompileShader(const GLuint& p_type,
                             const std::string& p_source)
{
	const char* pureSource = p_source.data();
	GLuint shaderId = glCreateShader(p_type);
	glShaderSource(shaderId, 1, &pureSource, nullptr);
	glCompileShader(shaderId);

	Debug::OpenGL::Shader::ShaderDebug::CheckShaderErrors(shaderId, p_type);

	return shaderId;
}

std::string Shader::ShaderProgramSource(const std::string_view& p_filepath)
{
	std::string code;
	std::ifstream file;
	std::stringstream dataStream;

	try
	{
		file.open(std::string(p_filepath));
		dataStream << file.rdbuf();
		code = dataStream.str();
		file.close();
	}
	catch (std::ifstream::failure& exception)
	{
		std::cout << "File not successfully read";
		std::cout << exception.what();
	}

	return code;
}

GLuint Rendering::Resources::Shader::GetId() const
{
	return m_id;
}

const std::string& Shader::GetVertexShaderPath() const
{
	return m_sourceVertexShader;
}

const std::string& Shader::GetFragmentShaderPath() const
{
	return m_sourceFragmentShader;
}
