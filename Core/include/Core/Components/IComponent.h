#pragma once

namespace Core::Components
{
	class IComponent
	{
	public:
		virtual ~IComponent() = default;
		virtual void Update(const double p_dt) = 0;
		// Serialize every components for tinyxml
	};
}
