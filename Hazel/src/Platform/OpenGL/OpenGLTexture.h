#pragma once

#include <string>

#include "Hazel/Renderer/Texture.h"

namespace Hazel {
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const char* path);
		virtual ~OpenGLTexture2D() override;

		virtual int getWidth() const override;
		virtual int getHeight() const override;

		virtual void bind(int num) const override;

	private:
		std::string m_path;
		int m_width, m_height;
		unsigned int m_textureId;
	};
}