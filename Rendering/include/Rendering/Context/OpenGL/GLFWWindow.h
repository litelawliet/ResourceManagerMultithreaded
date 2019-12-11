#pragma once
#include <GLFW/glfw3.h>
#include <Rendering/Export.h>
#include <Rendering/Context/IWindow.h>

namespace Rendering::Context::OpenGL
{
	class API_RENDERING GLFWWindow final : public IWindow
	{
	public:
		GLFWWindow(unsigned int p_width, unsigned int p_height) noexcept;
		~GLFWWindow() noexcept;

		bool ShouldClose() const noexcept override;
		const std::any Data() const noexcept override;
		unsigned int GetWidth() const { return m_width; }
		unsigned int GetHeight() const { return m_height; }
		static GLFWWindow* GetInstance() noexcept;

	private:
		unsigned int m_width;
		unsigned int m_height;
		void BindKeyCallback() const noexcept;
		void BindMouseButtonCallback() const noexcept;
		void BindCloseWindowCallback() const noexcept;

		static GLFWWindow* m_instance;
		GLFWwindow* m_glfwWindow;
	};
}
