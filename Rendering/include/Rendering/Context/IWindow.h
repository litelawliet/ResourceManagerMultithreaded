#pragma once
#include <Rendering/Export.h>
#include <any>
#include <Rendering/Utils/Event.h>

namespace Rendering::Context
{
	class API_RENDERING IWindow
	{
	public:

		Utils::Event<int> m_keyPressedEvent;
		Utils::Event<int> m_keyReleasedEvent;
		Utils::Event<int> m_mouseButtonPressedEvent;
		Utils::Event<int> m_mouseButtonReleasedEvent;

		virtual ~IWindow() = default;

		virtual bool ShouldClose() const noexcept = 0;
		virtual const std::any Data() const noexcept = 0;
	};
}
