#pragma once
#include <Rendering/Export.h>
#include <glm/vec3.hpp>

namespace Rendering::LowRenderer
{
	struct API_RENDERING LightSource
	{
		glm::vec3 Position;
		glm::vec3 Color;
		glm::vec3 Direction;
		int Type;
		float Constant;
		float Linear;
		float Quadratic;
		float Cutoff;
		float OuterCutoff;
		float Intensity;
	};
}
