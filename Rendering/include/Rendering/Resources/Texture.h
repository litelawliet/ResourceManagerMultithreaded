#pragma once
#include <Rendering/Export.h>

namespace Rendering::Resources
{
	class API_RENDERING Texture
	{
	public:
		Texture();
		~Texture();

		void Bind(const int p_slot) const;
		void Unbind() const;

		void Load(const std::string& p_filepath, const int p_slot);

		inline bool IsTextureSet() const { return m_loaded; }

		inline const std::string& GetTexturePath() const;

	private:
		GLuint m_id{0u};
		std::string m_texturePath;
		bool m_loaded;
	};
}
